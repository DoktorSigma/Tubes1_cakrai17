Apa itu FSM
FSM atau Finite State Machine adalah suatu model komputasi yang berdasarkan mesin hipotesis yang terbuat
dari satu atau lebih status keaddan. Hanya satu status yang dapat aktif setiap waktunya dan mesin harus
melakukan transisi status keadaan jika ingin melakukan aksi atau fungsi yang berbeda.

Implementasinya adalah untuk mensimulasikan logika sekuensial atau mengontrol arus pengeksekusian fungsi. 
FSM bisa digunakan dalam banyak bidang, seperti matematika, kecerdasan buatan, video games, linguistik, 
dan lain hal-nya.


Implementasi: 

#include <iostream>  : Untuk input/output, seperti 'cout' (cetak ke layar) dan 'cin' (baca dari keyboard)
#include <string>    : Untuk menggunakan teks (tipe string)
#include <cstdint>   : Untuk jenis angka tertentu (seperti uint32_t)
#include <chrono>    : Untuk bekerja dengan waktu (misalnya, untuk fungsi millis nantinya)
#include <vector>    : Untuk membuat daftar yang bisa bertambah/berkurang (dinamis)
#include <limits>    : Untuk membantu membersihkan input keyboard (seperti enter, \n)
#include <thread>    : Untuk mengontrol waktu pada kode, (misalnya fungsi sleep_for() yang menghentikan kode sama 
                       halnya seperti fungsi delay() pada arduino)



Enum `SystemState`: Mendefinisikan kemungkinan status sistem: `INIT`, `IDLE`, `MOVEMENT`, `SHOOTING`, `CALCULATION`, `ERROR`, dan `STOPPED`.
Fungsi `millis()`: Fungsi bantu global yang mengembalikan waktu saat ini dalam milidetik, digunakan untuk melacak heartbeat dan waktu masuk status.
Kelas `FSM`:
`currentState`: Menyimpan status FSM saat ini.
`lastHeartbeat`: Mencatat waktu terakhir (dalam milidetik) peristiwa penting atau transisi status terjadi.
`delay`: Penundaan yang dapat dikonfigurasi (dalam milidetik) untuk setiap transisi atau pembaruan status.
`errorCount`: Melacak jumlah erro* yang ditemui.
`stateHistory`: Sebuah `std::vector` dari `std::pair<SystemState, uint32_t>` untuk mencatat urutan status dan waktu masuknya.
`moveCount`: Melacak jumlah gerakan yang dilakukan.



Konstruktor (`FSM()` dan `FSM(uint32_t delay)`)**:
     Menginisialisasi `currentState` ke `INIT`, `lastHeartbeat` dan `errorCount` ke 0, serta `moveCount` ke 0.
     Vektor `stateHistory` dikosongkan untuk memastikan awal yang bersih.
     Konstruktor berparameter juga mengatur `delay` awal.
     
Destruktor (`~FSM()`): Mengosongkan vektor `stateHistory` untuk melepaskan memori dan mencetak pesan konfirmasi.

`millis()`: Diimplementasikan menggunakan `std::chrono::high_resolution_clock` untuk menyediakan nilai milidetik

`transitionToState(SystemState newState)`:
     Memperbarui `currentState` ke `newState`.
     Mengatur `lastHeartbeat` ke waktu saat ini.
     Memanggil `addStateToHistory()` untuk mencatat transisi.
     
`start()`:
     Menginisialisasi FSM dengan transisi ke `INIT`.
     Memasuki loop `while` yang berlanjut selama FSM tidak dalam status `STOPPED`.
     Di dalam loop, `update()` dipanggil, dan `std::this_thread::sleep_for` digunakan untuk memperkenalkan `delay` antar pembaruan
     Memanggil `shutdown()` setelah status `STOPPED` tercapai.
     
`update()`:
    * Bertindak sebagai pusat pengiriman logika FSM.
    * Memeriksa `currentState` dan memanggil metode `perform...()` yang sesuai.
    * Pemeriksaan `(millis() - lastHeartbeat < delay)` untuk memastikan delay sesuai
    
`performInit()`:
     Mencetak pesan inisialisasi.
     Mengatur `delay` default ke 1000 milidetik.
     Mentransisikan FSM ke status `IDLE`.
     Mencetak status saat ini.
     
`performProcess()` (Implementasi CLI):
    dipanggil ketika FSM berada dalam status `IDLE`.
    Menyajikan menu melalui output standar 
    Membaca masukan pengguna menggunakan `cin`.
   Penanganan Masukan**: `cin.ignore(numeric_limits<streamsize>::max(), '\n');` digunakan setelah `cin >> choiceStr;` untuk membersihkan buffer masukan. Ini mencegah karakter baris baru yang tersisa memengaruhi operasi `cin` berikutnya.

`performMovement()`:
     Menaikkan `moveCount`.
     Jika `moveCount` mencapai 3, ia mentransisi ke `SHOOTING`.
     Jika tidak, ia mentransisi kembali ke `IDLE`.
     
`performShooting()`:
     Mereset `moveCount` ke 0. (ke awal)
     Mentransisi kembali ke `IDLE`.
     
`performCalculation()`:
     Jika `moveCount` adalah 0, ia mentransisi ke `ERROR`.
     Jika tidak, ia mentransisi kembali ke `IDLE`.
    
`performErrorHandling()`:
     Menaikkan jumlah `errorCount`.
     Jika `errorCount` mencapai 3, ia mentransisi ke `STOPPED`.
     Jika tidak, ia mentransisi kembali ke `IDLE`.
`shutdown()`:
     Mencetak pesan penutupan.
     Mengosongkan vektor `stateHistory`. (seperti awal)

`printStatus()` dan `printStateHistory()`: untuk menampilkan status saat ini dan urutan riwayat status.



pada file `main.cpp`, kita akan menggunakan kelas `FSM` yang dirujuk oleh header pada file `fsm.hpp`:
 
Ini membuat objek `FSM` bernama `robotFSM` dengan penundaan awal 2000 milidetik.
Ini memanggil `robotFSM.start()` untuk memulai operasi FSM.

Implementasi dan pembuatan kode dilakukan dengan bantuan AI Tools seperti Gemini dan proses pembelajaran
dan materi berasal dari Handbook CaKrai 17 dan Gemini AI pula. Semoga dikedepannya, saya dapat mendalami lebih
mengenai OOP dan C++ secara keseluruhan. Sekian, Terimakasih!!
