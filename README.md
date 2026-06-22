# CatServer & CatClient

Сетевой сервер и клиент для игры «Накорми и погладь кота». Сервер принимает UDP и TCP соединения, клиент — терминальное TUI-приложение. Инфраструктура поднимается через Docker (сервер, DNS, FTP).


---

## Архитектура

```
┌────────────────────────────────────────────┐
│               Docker (catnet)              │
│                                            │
│  ┌──────────────┐   ┌──────────────────┐   │
│  │  CatServer   │   │   BIND9 (DNS)    │   │
│  │ 172.20.0.2   │   │  172.20.0.3      │   │
│  │              │   │  cat.local       │   │
│  │ UDP :32132   │   │  SRV записи      │   │
│  │ TCP :45341   │   └──────────────────┘   │
│  │              │   ┌──────────────────┐   │
│  │ stats/  ─────┼───►  FTP (vsftpd)    │   │
│  │ stats.csv    │   │  172.20.0.4      │   │
│  └──────────────┘   │  :21             │   │
│                     └──────────────────┘   │
└────────────────────────────────────────────┘
         ▲ порты проброшены на хост
         │
┌────────┴──────────┐
│    CatClient      │
│                   │
│  UDP :55000       │
│  TCP not fixed    │
└───────────────────┘
```

### Компоненты сервера

| Компонент | Описание |
|---|---|
| `Controller` | Два потока: UDP и TCP. Вся бизнес-логика |
| `CatService` | Хранит имя кота и любимую еду (задаётся в коде) |
| `FeedQueue` | Парсит UDP-сообщения, собирает фрагменты |
| `PetQueue` | Парсит TCP-сообщения, собирает фрагменты |
| `Logger` | Потокобезопасный логгер в `log.txt` |
| `UDPTransceiver` | UDP-сокет (bind + recvfrom/sendto) |
| `TCPServer` | TCP-сервер (bind + listen + accept) |

### Компоненты клиента

| Компонент | Описание |
|---|---|
| `NetworkController` | UDP и TCP соединения, очереди входящих сообщений |
| `ClientUI` | Терминальный интерфейс на FTXUI |
| `TCPClient` | TCP-коннект с резолвингом доменных имён через `getaddrinfo` |
| `UDPTransceiver` | UDP-сокет клиента |

---

## Структура проекта

```
.
├── CatClient/                  # Docker-инфраструктура
│   ├── docker-compose.yml
│   ├── catserver/           # Dockerfile сервера
│   ├── dns/                 # BIND9: named.conf, зона cat.local
│   ├── ftp/                 # vsftpd: конфиг FTP
│   ├── cs/                  # Исходники CatServer
│   │   ├── CMakeLists.txt
│   │   ├── config/
│   │   │   └── cat.json     # Имя кота и любимая еда
│   │   ├── libs/net/        # UDP и TCP библиотеки
│   │   └── src/
│   │       ├── controller/
│   │       ├── cat_service/
│   │       ├── feed_queue/
│   │       ├── pet_queue/
│   │       ├── food/
│   │       ├── logger/
│   │       └── main.cpp
│   └── stats/
│       └── stats.csv        # Статистика (пишет сервер, отдаёт FTP)
│
└── CatClient/               # Клиентское приложение
    ├── CMakeLists.txt
    ├── dns_on.sh            # Включить Docker DNS
    ├── dns_off.sh           # Выключить Docker DNS
    ├── get_stats.sh         # Скачать статистику с FTP
    ├── libs/net/            # UDP и TCP библиотеки
    └── src/
        ├── controller/
        ├── clientui/
        └── main.cpp         # Конфигурация — здесь
```

---

## Зависимости

### Сервер (собирается внутри Docker)
| Пакет | Версия |
|---|---|
| Docker | >= 20.10 |
| Docker Compose | >= 2.0 |

Всё остальное (g++, cmake, bind9, vsftpd) ставится автоматически в контейнерах.

### Клиент (собирается на хосте)
| Пакет | Установка |
|---|---|
| g++ с поддержкой C++20 | `sudo apt install g++` |
| cmake >= 3.10 | `sudo apt install cmake` |
| FTXUI | скачивается автоматически через FetchContent |
| curl | `sudo apt install curl` (для get_stats.sh) |

---

## Сервер — запуск через Docker

### 1. Настройка кота

Имя и любимая еда задаются статически в `docker/cs/src/cat_service/CatService.h`:

```cpp
static constexpr const char * NAME = "Fluffy";

static const std::unordered_set<FoodType> & liked_food() {
    static const std::unordered_set<FoodType> foods = {
        FoodType::FISH,
        FoodType::MILK,
    };
    return foods;
}
```

Доступные типы еды: `FISH`, `MILK`, `MEAT`, `BREAD`, `CARROT`, `BEER`, `MARTINI`.

### 2. Запуск

```bash
cd docker/
docker compose up --build
```

Флаг `--build` нужен только при первом запуске или после изменений в коде. Последующие запуски:

```bash
docker compose up
```

В фоне:
```bash
docker compose up -d
```

### 3. Проверка что всё поднялось

```bash
docker compose ps
```

Должны быть три контейнера со статусом `running`:
```
catserver   Up   0.0.0.0:32132->32132/udp, 0.0.0.0:45341->45341/tcp
catdns      Up   0.0.0.0:53->53/udp
catftp      Up   0.0.0.0:21->21/tcp
```

Логи сервера:
```bash
docker logs catserver -f
```

---

## Клиент — сборка и запуск

### 1. Сборка

```bash
cd CatClient/
cmake -S . -B build
cmake --build build
```

### 2. Настройка адреса сервера

Открой `src/main.cpp` и поправь константы вверху файла:

```cpp
static const std::string SERVER_HOST = "catserver.cat.local"; 
static const uint32_t    SERVER_UDP  = 32132;
static const uint32_t    SERVER_TCP  = 45341;
static const std::string CLIENT_IP   = "0.0.0.0";
static const uint32_t    CLIENT_UDP  = 55000;
```

### 3. Подключение DNS (если используешь доменное имя)

Чтобы `catserver.cat.local` резолвился, нужно направить DNS-запросы к Docker-контейнеру. После `docker compose up`:

```bash
./dns_on.sh
```

Скрипт сделает бэкап `/etc/resolv.conf` и добавит `127.0.0.1` как DNS-сервер.

### 4. Запуск клиента

```bash
./build/catclient
```

### Интерфейс клиента

```
┌─────────────────────────────┬──────────────────────────────┐
│ Cat Network Tool            │ LOGS                         │
│─────────────────────────────│──────────────────────────────│
│ Local  IP:   0.0.0.0        │ WELCOME TO CATCLIENT!        │
│ Local  port: 55000          │ Server: catserver.cat.local  │
│─────────────────────────────│ UDP -> 32132                 │
│ Remote: catserver:32132     │ TCP -> 45341                 │
│─────────────────────────────│ SEND UDP: @Alex - Fish~      │
│ Message: @Alex - Fish~      │ RECV UDP: Eaten by the Cat   │
│─────────────────────────────│                              │
│ Protocol:                   │                              │
│ ○ TCP                       │                              │
│ ● UDP                       │                              │
│─────────────────────────────│                              │
│ [ SEND ]                    │                              │
└─────────────────────────────┴──────────────────────────────┘
```

---

## Протокол взаимодействия

### Feed the Cat — UDP (порт 32132)

Формат сообщения:
```
@<UserID> - <Food>~
```

Примеры:
```
@Alex - Fish~
@007 - Martini~
@Zavulon1237 - Meat~
```

Ответы сервера:
| Ситуация | Ответ |
|---|---|
| Еда понравилась | `Eaten by the Cat` |
| Еда не понравилась | `Ignored by the Cat` |

#### Фрагментация

Если сообщение не помещается в датаграмму, оно разбивается на фрагменты. Каждый фрагмент кроме последнего завершается номером: `~<N>`, последний — просто `~`.

```
Клиент --> "@Al~0"          Сервер --> "The Cat is amused by #0"
Клиент --> "ex - Fi~1"      Сервер --> "The Cat is amused by #1"
Клиент --> "sh~"            Сервер --> "Eaten by the Cat"
```

### Pet the Cat — TCP (порт 45341)

После установки соединения отправляй ID пользователя:
```
@<UserID>~
```

Можно отправить несколько ID подряд в одном пакете:
```
@Alex~@007~@Alex~
```

Ответы:
| Ситуация | Ответ |
|---|---|
| Пользователь кормил кота (успешно) | `Tolerated by the Cat` |
| Пользователь не кормил кота | `Scratched by the Cat` |

Кот терпит **5 поглаживаний**, после чего сервер закрывает соединение.

Фрагментация тоже поддерживается:
```
Клиент --> "@Ale"
Клиент --> "x~"             Сервер --> "Tolerated by the Cat"
```


---

## Статистика через FTP

CatServer записывает каждое событие в `docker/stats/stats.csv`.

Скачать файл:
```bash
./get_stats.sh
```

Или с указанием имени файла:
```bash
./get_stats.sh my_stats.csv
```

Формат файла:
```csv
timestamp,event,user,detail
2024-06-22 14:00:01,feed_accepted,Alex,Fish
2024-06-22 14:00:05,feed_rejected,007,Martini
2024-06-22 14:00:10,pet_tolerated,Alex,172.20.0.5:54321
2024-06-22 14:00:15,pet_scratched,007,172.20.0.5:54322
```

Типы событий:
| Событие | Описание |
|---|---|
| `feed_accepted` | Кот съел еду |
| `feed_rejected` | Кот проигнорировал еду |
| `pet_tolerated` | Кот потерпел поглаживание |
| `pet_scratched` | Кот царапнул |

Также можно скачать вручную:
```bash
curl -o stats.csv ftp://127.0.0.1/stats.csv
```

---

## Как остановить всё

### Остановить Docker-контейнеры

```bash
cd docker/
docker compose down
```

### Восстановить системный DNS

```bash
cd CatClient/
./dns_off.sh
```

### Полная очистка (включая образы)

```bash
cd docker/
docker compose down --rmi all --volumes
```

---

## DNS — SRV-записи

Домен `cat.local` содержит SRV-записи для автообнаружения сервисов:

```
_feed-the-cat._udp.cat.local.  SRV  5 5 32132 catserver.cat.local.
_pet-the-cat._tcp.cat.local.   SRV  5 5 45341 catserver.cat.local.
```

Проверка:
```bash
dig @127.0.0.1 SRV _feed-the-cat._udp.cat.local
dig @127.0.0.1 SRV _pet-the-cat._tcp.cat.local
```
