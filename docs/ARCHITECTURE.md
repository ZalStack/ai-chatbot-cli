# Architecture Documentation

## Gambaran Umum

AI Chatbot CLI adalah aplikasi terminal berbasis C++ yang menggabungkan pemrograman tingkat rendah dengan kecerdasan buatan modern. Aplikasi ini dibangun dengan arsitektur modular yang memisahkan setiap komponen ke dalam kelas terpisah.

## Komponen Utama

Aplikasi terdiri dari beberapa modul independen yang saling berkomunikasi. Modul Chatbot bertindak sebagai orkestrator yang mengkoordinasikan semua komponen lainnya. Modul API Handler bertanggung jawab untuk komunikasi dengan Groq API menggunakan libcurl.

Modul Database menangani semua operasi penyimpanan dan pengambilan data dari MySQL. Modul History mengelola konteks percakapan dalam memori dan menyediakan akses ke riwayat yang tersimpan. Modul Utils menyediakan fungsi-fungsi pembantu seperti parsing string dan manajemen environment.

## Alur Kerja

Saat pengguna mengetik pesan, input dikirim ke Chatbot yang kemudian menambahkannya ke History. History menyimpan pesan ke database dan menyiapkan konteks percakapan terbaru untuk dikirim ke API.

API Handler mengirim request ke Groq dengan konteks percakapan dan system prompt yang aktif. Respons dari API kemudian ditampilkan ke pengguna dan disimpan kembali ke database melalui History.

## Pola Desain

Aplikasi menggunakan dependency injection dimana setiap komponen menerima dependensinya melalui constructor. Smart pointers digunakan untuk manajemen memori otomatis, dan mutex melindungi akses database dari race condition dalam lingkungan multi-threaded.

## Ekstensibilitas

Arsitektur modular memudahkan penambahan fitur baru. Provider AI baru dapat ditambahkan dengan membuat implementasi API Handler baru. Database backend dapat diganti dengan mengimplementasikan interface Database yang sama.