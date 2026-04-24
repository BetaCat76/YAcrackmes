/*
 * c03_TLS1 — TLS-callback anti-debug crackme
 *
 * The main flow reads a license key, decrypts it with a simple rolling-XOR
 * algorithm, and compares the result to a hard-coded checksum.  If they match
 * the flag is printed.
 *
 * A TLS callback fires before main() and spawns a background thread that
 * continuously checks whether x64dbg (or x32dbg) is running.  If the debugger
 * is detected the process is killed immediately.
 *
 * Decryption algorithm
 * --------------------
 *   The license key is 16 upper-hex digits (dashes optional, e.g. XXXX-XXXX-XXXX-XXXX).
 *   They are parsed into 8 bytes.  Each byte i is XOR-ed with (0x42 + i * 0x13).
 *   The 8 decrypted bytes must equal:  "WELCOME!"  (0x57 0x45 0x4C 0x43 0x4F 0x4D 0x45 0x21)
 *
 *   Correct key:  1510-2438-C1EC-F1E6
 */

#include <Windows.h>
#include <TlHelp32.h>

#include <array>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <print>
#include <string>

// ── Flag & checksum ───────────────────────────────────────────────────────────

static constexpr const char* kFlag = "FLAG{TLS_C4llb4ck_K1ll5_D3bugg3r}";

// Expected bytes after decryption: "WELCOME!"
static constexpr std::array<uint8_t, 8> kChecksum = {
    0x57, 0x45, 0x4C, 0x43, 0x4F, 0x4D, 0x45, 0x21
};

// ── License verification ──────────────────────────────────────────────────────

static bool verify_license(const std::string& input)
{
    // Strip dashes/spaces and upper-case
    std::string clean;
    clean.reserve(16);
    for (char c : input) {
        if (c == '-' || c == ' ') continue;
        clean += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    if (clean.size() != 16) return false;

    // Parse 8 hex byte pairs
    uint8_t bytes[8];
    for (int i = 0; i < 8; ++i) {
        char hex[3] = { clean[i * 2], clean[i * 2 + 1], '\0' };
        char* endp  = nullptr;
        unsigned long val = std::strtoul(hex, &endp, 16);
        if (endp != hex + 2) return false;
        bytes[i] = static_cast<uint8_t>(val);
    }

    // Decrypt: decrypted[i] = bytes[i] XOR (0x42 + i * 0x13)
    for (int i = 0; i < 8; ++i) {
        if ((bytes[i] ^ static_cast<uint8_t>(0x42 + i * 0x13)) != kChecksum[i])
            return false;
    }
    return true;
}

// ── Anti-debug helpers ────────────────────────────────────────────────────────

static bool is_x64dbg_running()
{
    // 1. IsDebuggerPresent (catches attaching via most debuggers)
    if (IsDebuggerPresent())
        return true;

    // 2. Enumerate running processes and look for x64dbg.exe / x32dbg.exe
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W pe{};
        pe.dwSize = sizeof(pe);
        if (Process32FirstW(snap, &pe)) {
            do {
                // Lower-case the name for a case-insensitive comparison
                wchar_t lower[MAX_PATH];
                std::size_t len = std::wcslen(pe.szExeFile);
                for (std::size_t k = 0; k < len; ++k)
                    lower[k] = static_cast<wchar_t>(std::towlower(pe.szExeFile[k]));
                lower[len] = L'\0';

                if (std::wcscmp(lower, L"x64dbg.exe") == 0 ||
                    std::wcscmp(lower, L"x32dbg.exe") == 0) {
                    CloseHandle(snap);
                    return true;
                }
            } while (Process32NextW(snap, &pe));
        }
        CloseHandle(snap);
    }

    // 3. Look for the top-level window that x64dbg creates (Qt5 window class)
    if (FindWindowA("Qt5152QWindowIcon", nullptr) != nullptr)
        return true;

    return false;
}

// ── Anti-debug thread (launched by the TLS callback) ─────────────────────────

static DWORD WINAPI AntiDebugThread(LPVOID)
{
    for (;;) {
        if (is_x64dbg_running())
            TerminateProcess(GetCurrentProcess(), 0xDEAD);
        Sleep(500);
    }
}

// ── TLS callback ──────────────────────────────────────────────────────────────

static void NTAPI TlsCallback(PVOID /*hModule*/, DWORD dwReason, PVOID /*pContext*/)
{
    // Only DLL_PROCESS_ATTACH spawns the thread; DLL_THREAD_ATTACH calls are
    // intentionally ignored to avoid launching redundant monitoring threads.
    if (dwReason == DLL_PROCESS_ATTACH) {
        HANDLE hThread = CreateThread(nullptr, 0, AntiDebugThread, nullptr, 0, nullptr);
        if (hThread != nullptr)
            CloseHandle(hThread);
    }
}

// Register the TLS callback in the .CRT$XLB section so the loader invokes it
#pragma comment(linker, "/INCLUDE:_tls_used")
#pragma data_seg(".CRT$XLB")
extern "C" PIMAGE_TLS_CALLBACK pTlsCallback = TlsCallback;
#pragma data_seg()

// ── Entry point ───────────────────────────────────────────────────────────────

int main()
{
    std::println("=== License Key Verifier ===");
    std::println("Enter license key (format: XXXX-XXXX-XXXX-XXXX):");

    std::string key;
    std::getline(std::cin, key);

    if (verify_license(key))
        std::println("Correct! {}", kFlag);
    else
        std::println("Wrong license key. Try again.");

    return 0;
}
