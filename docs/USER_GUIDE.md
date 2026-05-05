# User Guide

## Memulai Aplikasi

Setelah instalasi selesai, jalankan chatbot dari terminal dengan menjalankan executable yang dihasilkan. Aplikasi akan secara otomatis memuat konfigurasi dari file .env dan menghubungkan ke database serta Groq API.

## Berinteraksi dengan AI

Ketik pesan apa saja dan tekan Enter untuk mengirim ke AI. AI akan merespons berdasarkan konteks percakapan dan system prompt yang aktif. Percakapan bersifat kontekstual, artinya AI mengingat pesan sebelumnya dalam sesi yang sama.

## Perintah Khusus

Aplikasi menyediakan berbagai perintah yang diawali dengan garis miring. Gunakan /help untuk melihat daftar lengkap perintah yang tersedia. Perintah /history menampilkan riwayat percakapan, sementara /clear menghapus riwayat.

## Mengelola System Prompt

System prompt menentukan kepribadian dan perilaku AI. Gunakan /prompts untuk melihat prompt yang tersedia dan /prompt namaprompt untuk beralih. Anda juga dapat membuat prompt kustom dengan perintah /save.

## Bekerja dengan Session

Setiap kali aplikasi dijalankan, session baru dibuat secara otomatis. Gunakan /sessions untuk melihat session yang tersedia dan /switch untuk berpindah antar session. Setiap session memiliki riwayat percakapan yang terpisah.

## Ekspor Data

Percakapan dapat diekspor ke file teks dengan perintah /export. File akan disimpan dengan nama yang mengandung session ID untuk identifikasi mudah.

## Menutup Aplikasi

Gunakan /quit atau /exit untuk keluar dari aplikasi dengan aman. Semua data percakapan tetap tersimpan di database dan dapat diakses kembali di sesi berikutnya.