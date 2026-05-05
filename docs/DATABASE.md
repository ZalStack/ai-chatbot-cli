# Database Documentation

## Struktur Database

Aplikasi menggunakan MySQL atau MariaDB untuk menyimpan riwayat percakapan dan sistem prompt kustom. Database terdiri dari dua tabel utama yang saling mendukung fitur chatbot.

## Tabel Conversations

Tabel ini menyimpan setiap pesan dalam percakapan. Setiap pesan memiliki session ID yang menghubungkannya dengan sesi percakapan tertentu, sehingga pengguna dapat memiliki banyak sesi percakapan yang terpisah.

Setiap baris mencatat peran pengirim (user, assistant, atau system), konten pesan, dan timestamp otomatis. Indeks pada session_id dan timestamp memastikan pengambilan data yang cepat saat menampilkan riwayat percakapan.

## Tabel Custom Prompts

Tabel ini menyimpan berbagai system prompt yang dapat dipilih pengguna. Setiap prompt memiliki nama unik dan teks prompt yang mendefinisikan kepribadian atau peran AI.

Satu prompt dapat ditandai sebagai aktif, yang akan digunakan sebagai system prompt default. Fitur ini memungkinkan pengguna beralih antara berbagai mode seperti programmer, teacher, atau asisten umum.

## Keamanan Data

Aplikasi menggunakan parameterized queries melalui fungsi escape string MySQL untuk mencegah SQL injection. Semua input pengguna di-escape sebelum dimasukkan ke dalam query.

## Fallback Mode

Jika koneksi database gagal, aplikasi tetap dapat berjalan dalam mode terbatas tanpa penyimpanan riwayat. Fitur chat dengan AI tetap berfungsi normal, namun riwayat percakapan tidak akan tersimpan.