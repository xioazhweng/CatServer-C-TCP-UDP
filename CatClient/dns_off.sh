#!/bin/bash
# Восстанавливает системный /etc/resolv.conf

RESOLV="/etc/resolv.conf"
BACKUP="/etc/resolv.conf.bak"

if [ ! -f "$BACKUP" ]; then
    echo "Бэкап не найден — DNS уже выключен?"
    exit 1
fi

sudo cp "$BACKUP" "$RESOLV"
sudo rm "$BACKUP"

echo "DNS восстановлен."
cat "$RESOLV"
