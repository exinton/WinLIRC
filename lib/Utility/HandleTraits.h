#pragma once

/// Traits for handles returned by CreateFile.
struct FileTraits
{
	typedef HANDLE HandleType;

	static HandleType invalidValue() { return INVALID_HANDLE_VALUE; }
	static void close(HandleType h) { ::CloseHandle(h); }
};

typedef FileTraits SerialPortTraits;

/// Traits for handles returned by WSACreateEvent.
struct WsaEventTraits
{
	typedef WSAEVENT HandleType;

	static HandleType invalidValue() { return WSA_INVALID_EVENT; }
	static void close(HandleType h) { ::WSACloseEvent(h); }
};