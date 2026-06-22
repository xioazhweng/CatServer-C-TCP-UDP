FROM ubuntu:24.04

RUN apt-get update && apt-get install -y vsftpd && \
    rm -rf /var/lib/apt/lists/*

# Анонимный FTP только для чтения — папка /ftp/pub
RUN mkdir -p /ftp/pub && chmod 755 /ftp/pub

COPY vsftpd.conf /etc/vsftpd.conf

EXPOSE 21
CMD ["vsftpd", "/etc/vsftpd.conf"]
