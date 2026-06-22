#!/bin/bash
# Скачивает stats.csv с FTP-сервера

FTP_HOST="127.0.0.1"
FILE="stats.csv"
OUT="${1:-stats.csv}"   # можно передать имя файла аргументом, по умолчанию stats.csv

echo "Скачиваю $FILE с ftp://$FTP_HOST ..."

curl -s --retry 3 \
     "ftp://$FTP_HOST/$FILE" \
     -o "$OUT"

if [ $? -eq 0 ] && [ -s "$OUT" ]; then
    echo "Сохранено: $OUT"
    echo ""
    cat "$OUT"
else
    echo "Ошибка: не удалось скачать файл. Проверьте, что docker compose запущен."
    exit 1
fi
