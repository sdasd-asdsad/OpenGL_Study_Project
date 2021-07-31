#pragma once

#include <fstream>
#include <queue>
#include <string>
#include <iostream>

class Logger
{
	bool bActive;
	bool bFileout;
	bool bConsoleout;
	size_t size_buffer;
	std::queue<std::string*> buffer;
public:
	Logger(bool _bActive, size_t _size_buffer,bool _bConsoleout, bool _bFileout) {
		this->bActive = _bActive;
		this->size_buffer = _size_buffer;
		this->bConsoleout = _bConsoleout;
		this->bFileout = _bFileout;
	}
	void log(std::string* ptr_str) {
		if (bActive) {
			/*
			this->buffer.push(ptr_str);
			if (buffer.size() >= size_buffer) {
				this->buffer.pop();
			}*/
			print(ptr_str);
		}
	}
	void print(std::string* ptr_str){
		if (bActive) {
			if (bConsoleout) {
				std::cout << *ptr_str << std::endl;
			}
			if (bFileout) {
				// do write on log file;
			}
		}
	}
};

Logger* logger; // allocated by EngineCore