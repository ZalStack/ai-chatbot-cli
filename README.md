# 🤖 AI Chatbot CLI

<div align="center">

![C++](https://img.shields.io/badge/C++-17-blue.svg?style=for-the-badge&logo=c%2B%2B)
![Groq](https://img.shields.io/badge/Groq-API-orange.svg?style=for-the-badge)
![MySQL](https://img.shields.io/badge/MySQL-Database-blue.svg?style=for-the-badge&logo=mysql)
![Docker](https://img.shields.io/badge/Docker-Ready-2496ED.svg?style=for-the-badge&logo=docker)
![Linux](https://img.shields.io/badge/Linux-Supported-success.svg?style=for-the-badge&logo=linux)
![Windows](https://img.shields.io/badge/Windows-Supported-blue.svg?style=for-the-badge&logo=windows)

**Chatbot CLI Canggih Berbasis C++ dengan Integrasi AI Groq API**

[Fitur](#-fitur) • [Instalasi](#-instalasi) • [Penggunaan](#-penggunaan) • [Dokumentasi](docs/) • [Kontribusi](#-kontribusi)

</div>

---

## 📖 Deskripsi

AI Chatbot CLI adalah aplikasi chatbot terminal yang dibangun dengan **C++** dan terintegrasi dengan **Groq AI API**. Project ini menggabungkan performa tinggi bahasa pemrograman low-level (C++) dengan kecerdasan buatan modern, menciptakan solusi yang cepat, efisien, dan powerful untuk otomatisasi lokal.

### 💡 Mengapa Project Ini Unik?

- **Performa Tinggi**: Dibangun dengan C++17, memberikan kecepatan eksekusi maksimal
- **Low-Level + AI**: Kombinasi langka antara pemrograman sistem dengan AI
- **Offline History**: Semua percakapan tersimpan di database lokal (MySQL/MariaDB)
- **Customizable**: Sistem prompt yang dapat disesuaikan untuk berbagai kebutuhan
- **Cross-Platform**: Berjalan di Linux dan Windows

---

## ✨ Fitur

### 🎯 Fitur Utama
- 💬 **Chat Real-time** - Berinteraksi dengan AI langsung dari terminal
- 🤖 **Integrasi Groq API** - Menggunakan model Llama 3.3 70B Versatile
- 📝 **History Percakapan** - Semua percakapan tersimpan otomatis di MySQL/MariaDB
- 🎨 **Custom System Prompts** - Ubah kepribadian AI sesuai kebutuhan
- 🔄 **Multi-Session** - Kelola banyak sesi percakapan sekaligus
- 📤 **Export Percakapan** - Simpan riwayat chat ke file teks
- 🐳 **Docker Support** - Deployment mudah dengan container

### 🛠️ Fitur Teknis
- ⚡ **C++17** - Performa tinggi dan efisien
- 🔒 **Thread-safe** - Koneksi database aman untuk concurrent access
- 📊 **Database MySQL/MariaDB** - Penyimpanan persisten dan dapat diandalkan
- 🎯 **Error Handling** - Graceful degradation jika database tidak tersedia
- 🌈 **Colored Output** - Tampilan terminal yang menarik dan informatif
- 🔧 **Cross-Platform** - Dukungan Linux (Fedora, Ubuntu, Arch) dan Windows

---

## 📋 Persyaratan Sistem

### Minimal Requirements
- **Compiler**: GCC 8+ atau MSVC 2019+
- **CMake**: 3.15+
- **RAM**: 512 MB
- **Disk**: 100 MB
- **Internet**: Koneksi untuk API Groq

### Dependencies
- **libcurl** - HTTP client
- **nlohmann/json** - JSON parser (otomatis diunduh)
- **MySQL/MariaDB** - Database (opsional, bisa tanpa database)
- **Docker** (opsional) - Untuk containerization

---

## 🚀 Instalasi

```bash
# Clone repository
git clone https://github.com/ZalStack/ai-chatbot-cli.git
cd ai-chatbot-cli

# Jalankan setup otomatis
chmod +x scripts/setup_linux.sh
./scripts/setup_linux.sh

# Jalankan chatbot
cd build && ./chatbot

# Jalankan Manual 
Make clean (untuk clear hasil compiler)

# Build Ulang 
Make 

# Jalankan Chatbot
Make run
