#!/bin/bash
# Подключает DNS-контейнер для резолвинга cat.local
# Работает через /etc/resolv.conf (без systemd-resolved)

RESOLV="/etc/resolv.conf"
BACKUP="/etc/resolv.conf.bak"

if [ -f "$BACKUP" ]; then
    echo "Бэкап уже существует — DNS уже включён? Сначала запусти dns_off.sh"
    exit 1
fi

echo "Создаю бэкап $RESOLV -> $BACKUP"
sudo cp "$RESOLV" "$BACKUP"

# Добавляем наш DNS первым, остальные оставляем как fallback
CURRENT=$(cat "$RESOLV")
echo "nameserver 127.0.0.1
search cat.local
$CURRENT" | sudo tee "$RESOLV" > /dev/null

echo "Тест резолвинга:"
dig +short catserver.cat.local

echo ""
echo "Готово. catserver.cat.local -> $(dig +short catserver.cat.local)"
