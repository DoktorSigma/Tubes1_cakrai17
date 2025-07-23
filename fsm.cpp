#include "fsm.hpp"
#include <limits>   
#include <thread>  
#include <chrono>   

// Fungsi bantu global untuk mendapatkan milidetik
uint32_t millis() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}


// Konstruktor default
FSM::FSM() : currentState(SystemState::INIT), lastHeartbeat(0), delay(0), errorCount(0), moveCount(0) {
    stateHistory.clear(); // Pastikan vektor kosong
}

// Konstruktor yang ditentukan pengguna
FSM::FSM(uint32_t initialDelay) : currentState(SystemState::INIT), lastHeartbeat(0), delay(initialDelay), errorCount(0), moveCount(0) {
    stateHistory.clear();
}
// Destruktor
FSM::~FSM() {
    stateHistory.clear(); 
    cout << "Objek FSM dihancurkan." << endl;
}

// Dapatkan status saat ini
SystemState FSM::getCurrentState() const {
    return currentState;
}

// Transisi ke status baru
void FSM::transitionToState(SystemState newState) {
    currentState = newState;
    lastHeartbeat = millis();
    addStateToHistory(newState, lastHeartbeat);
}

// Atur delay
void FSM::setDelay(uint32_t newDelay) {
    delay = newDelay;
}

// Dapatkan delay
void FSM::getDelay(uint32_t &currentDelay) const {
    currentDelay = delay;
}

// Atur jumlah error
void FSM::setErrorCount(int count) {
    errorCount = count;
}

// Dapatkan jumlah error
int FSM::getErrorCount() const {
    return errorCount;
}

// Atur jumlah gerakan
void FSM::setMoveCount(int count) {
    moveCount = count;
}

// Dapatkan jumlah gerakan
int FSM::getMoveCount() const {
    return moveCount;
}

// Tambahkan status ke riwayat
void FSM::addStateToHistory(SystemState state, uint32_t time) {
    stateHistory.emplace_back(state, time);
}

// Dapatkan riwayat status
vector<pair<SystemState, uint32_t>> FSM::getStateHistory() const {
    return stateHistory;
}

// Dapatkan waktu heartbeat terakhir
uint32_t FSM::getLastHeartbeat() const {
    return lastHeartbeat;
}

// Atur waktu heartbeat terakhir
void FSM::setLastHeartbeat(uint32_t heartbeat) {
    lastHeartbeat = heartbeat;
}

// Mulai FSM
void FSM::start() {
    cout << "FSM dimulai..." << endl;
    transitionToState(SystemState::INIT); // Mulai dengan status INIT

    while (currentState != SystemState::STOPPED) {
        update();
        if (delay > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    shutdown(); // Panggil shutdown saat FSM berhenti
}

// Perbarui status FSM
void FSM::update() {
    if (currentState != SystemState::INIT && (millis() - lastHeartbeat < delay)) {
        return; // Tunggu sampai penundaan berlalu
    }
    switch (currentState) {
        case SystemState::INIT:
            performInit();
            break;
        case SystemState::IDLE:
            performProcess();
            break;
        case SystemState::MOVEMENT:
            performMovement();
            break;
        case SystemState::SHOOTING:
            performShooting();
            break;
        case SystemState::CALCULATION:
            performCalculation();
            break;
        case SystemState::ERROR:
            performErrorHandling();
            break;
        case SystemState::STOPPED:
            // Ditangani oleh loop while di start()
            break;
        default:
            cout << "Status tidak dikenal!" << endl;
            transitionToState(SystemState::ERROR);
            break;
    }
    if (currentState != SystemState::STOPPED) { // Hindari memperbarui jika sudah berhenti untuk shutdown akhir
        lastHeartbeat = millis();
    }
}

// Cetak status saat ini
void FSM::printStatus() {
    cout << "--- Status FSM ---" << endl;
    cout << "Status Saat Ini: ";
    switch (currentState) {
        case SystemState::INIT: cout << "INIT"; break;
        case SystemState::IDLE: cout << "IDLE"; break;
        case SystemState::MOVEMENT: cout << "MOVEMENT"; break;
        case SystemState::SHOOTING: cout << "SHOOTING"; break;
        case SystemState::CALCULATION: cout << "CALCULATION"; break;
        case SystemState::ERROR: cout << "ERROR"; break;
        case SystemState::STOPPED: cout << "STOPPED"; break;
    }
    cout << endl;
    cout << "Heartbeat Terakhir: " << lastHeartbeat << " ms" << endl;
    cout << "Penundaan: " << delay << " ms" << endl;
    cout << "Jumlah Error: " << errorCount << endl;
    cout << "Jumlah Gerakan: " << moveCount << endl;
    cout << "------------------" << endl;
}

// Cetak riwayat status
void FSM::printStateHistory() {
    cout << "--- Riwayat Status ---" << endl;
    if (stateHistory.empty()) {
        cout << "Tidak ada riwayat tersedia." << endl;
    } else {
        for (const auto& entry : stateHistory) {
            cout << "Status: ";
            switch (entry.first) {
                case SystemState::INIT: cout << "INIT"; break;
                case SystemState::IDLE: cout << "IDLE"; break;
                case SystemState::MOVEMENT: cout << "MOVEMENT"; break;
                case SystemState::SHOOTING: cout << "SHOOTING"; break;
                case SystemState::CALCULATION: cout << "CALCULATION"; break;
                case SystemState::ERROR: cout << "ERROR"; break;
                case SystemState::STOPPED: cout << "STOPPED"; break;
            }
            cout << ", Waktu: " << entry.second << " ms" << endl;
        }
    }
    cout << "---------------------" << endl;
}

// Lakukan Inisialisasi
void FSM::performInit() {
    cout << "Menginisialisasi sistem..." << endl;
    delay = 1000; // Atur penundaan ke 1000 milidetik
    transitionToState(SystemState::IDLE); // Transisi ke IDLE setelah inisialisasi
    cout << "Sistem diinisialisasi. Memasuki status IDLE." << endl;
    printStatus();
}

// Lakukan Proses (Input)
void FSM::performProcess() {
    cout << "\n--- Status IDLE: Pilih tindakan ---" << endl;
    cout << "1. Tampilkan Status dan Riwayat Status" << endl;
    cout << "2. Bergerak (transisi ke MOVEMENT)" << endl;
    cout << "3. Menembak (transisi ke SHOOTING)" << endl;
    cout << "4. Menghitung (transisi ke CALCULATION)" << endl;
    cout << "5. Berhenti (transisi ke STOPPED)" << endl;
    cout << "Masukkan pilihan Anda: ";

    string choiceStr;
    cin >> choiceStr;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choiceStr == "1") {
        printStatus();
        printStateHistory();
        // Tetap di IDLE, tidak ada transisi status
    } else if (choiceStr == "2") {
        transitionToState(SystemState::MOVEMENT);
    } else if (choiceStr == "3") {
        transitionToState(SystemState::SHOOTING);
    } else if (choiceStr == "4") {
        transitionToState(SystemState::CALCULATION);
    } else if (choiceStr == "5") {
        transitionToState(SystemState::STOPPED);
    } else {
        cout << "Pilihan tidak valid. Tetap di IDLE." << endl;
    }
}

// Lakukan Gerakan
void FSM::performMovement() {
    cout << "Bergerak..." << endl;
    moveCount++;
    cout << "Jumlah Gerakan: " << moveCount << endl;
    if (moveCount >= 3) {
        cout << "Melakukan 3 gerakan. Transisi ke status SHOOTING." << endl;
        transitionToState(SystemState::SHOOTING);
    } else {
        cout << "Transisi ke status IDLE." << endl;
        transitionToState(SystemState::IDLE);
    }
}

// Lakukan Penembakan
void FSM::performShooting() {
    cout << "Menembak..." << endl;
    moveCount = 0; // Atur ulang moveCount
    cout << "Jumlah Gerakan direset ke: " << moveCount << endl;
    transitionToState(SystemState::IDLE);
    cout << "Transisi ke status IDLE." << endl;
}

// Lakukan Perhitungan
void FSM::performCalculation() {
    cout << "Melakukan perhitungan..." << endl;
    if (moveCount == 0) {
        cout << "Jumlah gerakan adalah 0. Transisi ke status ERROR." << endl;
        transitionToState(SystemState::ERROR);
    } else {
        cout << "Jumlah gerakan lebih besar dari 0. Transisi ke status IDLE." << endl;
        transitionToState(SystemState::IDLE);
    }
}

// Tangani Error
void FSM::performErrorHandling() {
    cout << "Terjadi error, melakukan penanganan error..." << endl;
    errorCount++;
    cout << "Jumlah Error: " << errorCount << endl;
    if (errorCount >= 3) {
        cout << "Jumlah error melebihi 3. Transisi ke status STOPPED." << endl;
        transitionToState(SystemState::STOPPED);
    } else {
        cout << "Transisi ke status IDLE." << endl;
        transitionToState(SystemState::IDLE);
    }
}

// Matikan FSM
void FSM::shutdown() {
    cout << "Sistem berhenti, mematikan..." << endl;
    stateHistory.clear(); // Bersihkan riwayat status
    cout << "Riwayat status dibersihkan." << endl;
}