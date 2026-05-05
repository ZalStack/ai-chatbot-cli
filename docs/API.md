# API Documentation

## Groq API Integration

Aplikasi ini menggunakan Groq API untuk menghasilkan respons AI. Groq menyediakan akses ke model bahasa besar (LLM) dengan kecepatan inferensi tinggi melalui infrastruktur LPU (Language Processing Unit) mereka.

## Endpoint
- **URL**: `https://api.groq.com/openai/v1/chat/completions`
- **Method**: POST
- **Authentication**: Bearer Token (API Key)

## Model yang Digunakan
- **llama-3.3-70b-versatile**: Model dari Meta dengan 70 miliar parameter, dioptimalkan untuk berbagai tugas percakapan dan pemrograman.

## Format Request
Request dikirim dalam format JSON dengan struktur yang kompatibel dengan OpenAI API. Setiap request mencakup array messages yang berisi percakapan antara user dan assistant, dilengkapi dengan system prompt untuk mengatur kepribadian AI.

## Format Response
Response dikembalikan dalam format JSON yang berisi pilihan respons dari model. Aplikasi mengekstrak konten dari field `choices[0].message.content` untuk ditampilkan ke pengguna.

## Error Handling
Aplikasi menangani berbagai kemungkinan error seperti invalid API key, rate limiting, dan network timeout. Setiap error ditampilkan dengan pesan yang informatif ke pengguna melalui terminal.