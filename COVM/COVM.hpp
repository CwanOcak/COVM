#pragma once

#include <windows.h>
#include <iostream>
#include <map>
#include <array>
#include <functional>


enum COVM_Instruction : std::uint8_t
{
	NOP, /* (32) == 0 */
	ADD, /* */
	SUB,
	MUL,
	DIV,
	AND,
	XOR,
	OR,
	NOT,
	INC,
	DEC,
	RET,
	MOV,

	PUSH,
	POP,

	LSTART,
	LEND,

	CALL,
	JMP
};

enum COVM_Register : std::uint32_t
{
	//16-bit 
	AX,
	BX,
	CX,
	DX,

	//32-bit
	EAX,
	ECX,
	EDX,
	EBX,
	ESP,
	EBP,
	ESI,
	EDI,

	//64-bit
	RAX,
	RCX,
	RDX,
	RBX,
	RSP,
	RBP,
	RSI,
	RDI,


};

enum COVM_Flag : std::uint32_t
{
	CF,
	PF,
	AF,
	ZF,
	SF,
	TF,
	IF,
	DF,
	OF
};


class COVM
{

public:

	COVM()
	{
		this->Stack = new std::uint8_t[64];
		this->StackCounter = 0;
		this->StackSize = 64;
		ZeroMemory(this->Stack, StackSize);
		this->OldCounter = 0;
	}

	COVM(std::uint32_t stackSize)
	{
		this->Stack = new std::uint8_t[stackSize];
		ZeroMemory(this->Stack, stackSize);
		this->StackCounter = 0;
		this->StackSize = stackSize;
		this->OldCounter = 0;
	}

	~COVM()
	{
		if (this->Stack)
			delete this->Stack;
	}

	void printRegister()
	{
		for (auto reg : Registers)
		{
			std::cout << reg.first << " | " << reg.second << std::endl;
		}
	}
	void printLabels()
	{
		for (auto lab : Labels)
		{
			std::cout << lab.first << " | " << lab.second << std::endl;
		}
	}

	void run(std::vector<std::uint8_t> Instructions)
	{
		int counter = 0, size_per_byte = 0;
		std::vector<std::uint8_t> FunctionBytes;
		for (int i = 0; i < Instructions.size(); i++)
		{
			switch (Instructions.at(i))
			{
			case NOP:
				break;
			case ADD:
				/*
					arguments:
					Is_register = 1/0
					Type = 1/2/4/32/64
				*/
				switch (Instructions[i + 1])
				{
				case 0:
					size_per_byte = Instructions[i + 2];
					switch (size_per_byte)
					{
					case 1:
						*reinterpret_cast<std::uint8_t*>(&Registers[Instructions[i + 3]]) += *reinterpret_cast<std::uint8_t*>(&Instructions[i + 4]);
						i += 3 + 1;
						break;
					case 2:
						*reinterpret_cast<std::uint16_t*>(&Registers[Instructions[i + 3]]) += *reinterpret_cast<std::uint16_t*>(&Instructions[i + 4]);
						i += 3 + 2;
						break;
					case 4:
						*reinterpret_cast<std::uint32_t*>(&Registers[Instructions[i + 3]]) += *reinterpret_cast<std::uint32_t*>(&Instructions[i + 4]);
						i += 3 + 4;
						break;
					case 8:
						*reinterpret_cast<std::uint64_t*>(&Registers[Instructions[i + 3]]) += *reinterpret_cast<std::uint64_t*>(&Instructions[i + 4]);
						i += 3 + 8;
						break;
					default:
						throw std::exception("invalid byte count");
					}
					break;
				case 1:
					Registers[Instructions[i + 2]] += Registers[Instructions[i + 3]];
					i += 2;
					break;
				default:
					throw std::exception("invalid mode");
				}
				break;
			case SUB:
				/*
					arguments:
					Is_register = 1/0
					Type = 1/2/4/32/64
				*/
				switch (Instructions[i + 1])
				{
				case 0:
					size_per_byte = Instructions[i + 2];
					switch (size_per_byte)
					{
					case 1:
						*reinterpret_cast<std::uint8_t*>(&Registers[Instructions[i + 3]]) -= *reinterpret_cast<std::uint8_t*>(&Instructions[i + 4]);
						i += 3 + 1;
						break;
					case 2:
						*reinterpret_cast<std::uint16_t*>(&Registers[Instructions[i + 3]]) -= *reinterpret_cast<std::uint16_t*>(&Instructions[i + 4]);
						i += 3 + 2;
						break;
					case 4:
						*reinterpret_cast<std::uint32_t*>(&Registers[Instructions[i + 3]]) -= *reinterpret_cast<std::uint32_t*>(&Instructions[i + 4]);
						i += 3 + 4;
						break;
					case 8:
						*reinterpret_cast<std::uint64_t*>(&Registers[Instructions[i + 3]]) -= *reinterpret_cast<std::uint64_t*>(&Instructions[i + 4]);
						i += 3 + 8;
						break;
					default:
						throw std::exception("invalid byte count");
					}
					break;
				case 1:
					Registers[Instructions[i + 2]] -= Registers[Instructions[i + 3]];
					i += 2;
					break;
				default:
					throw std::exception("invalid mode");
				}
				break;

			case MUL:
				/*
					arguments:
					Is_register = 1/0
					Type = 1/2/4/32/64
				*/
				switch (Instructions[i + 1])
				{
				case 0:
					size_per_byte = Instructions[i + 2];
					switch (size_per_byte)
					{
					case 1:
						*reinterpret_cast<std::uint8_t*>(&Registers[Instructions[i + 3]]) *= *reinterpret_cast<std::uint8_t*>(&Instructions[i + 4]);
						i += 3 + 1;
						break;
					case 2:
						*reinterpret_cast<std::uint16_t*>(&Registers[Instructions[i + 3]]) *= *reinterpret_cast<std::uint16_t*>(&Instructions[i + 4]);
						i += 3 + 2;
						break;
					case 4:
						*reinterpret_cast<std::uint32_t*>(&Registers[Instructions[i + 3]]) *= *reinterpret_cast<std::uint32_t*>(&Instructions[i + 4]);
						i += 3 + 4;
						break;
					case 8:
						*reinterpret_cast<std::uint64_t*>(&Registers[Instructions[i + 3]]) *= *reinterpret_cast<std::uint64_t*>(&Instructions[i + 4]);
						i += 3 + 8;
						break;
					default:
						throw std::exception("invalid byte count");
					}
					break;
				case 1:
					Registers[Instructions[i + 2]] *= Registers[Instructions[i + 3]];
					i += 2;
					break;
				default:
					throw std::exception("invalid mode");
				}
				break;
			case DIV:
				/*
					arguments:
					Is_register = 1/0
					Type = 1/2/4/32/64
				*/
				switch (Instructions[i + 1])
				{
				case 0:
					size_per_byte = Instructions[i + 2];
					switch (size_per_byte)
					{
					case 1:
						*reinterpret_cast<std::uint8_t*>(&Registers[Instructions[i + 3]]) /= *reinterpret_cast<std::uint8_t*>(&Instructions[i + 4]);
						i += 3 + 1;
						break;
					case 2:
						*reinterpret_cast<std::uint16_t*>(&Registers[Instructions[i + 3]]) /= *reinterpret_cast<std::uint16_t*>(&Instructions[i + 4]);
						i += 3 + 2;
						break;
					case 4:
						*reinterpret_cast<std::uint32_t*>(&Registers[Instructions[i + 3]]) /= *reinterpret_cast<std::uint32_t*>(&Instructions[i + 4]);
						i += 3 + 4;
						break;
					case 8:
						*reinterpret_cast<std::uint64_t*>(&Registers[Instructions[i + 3]]) /= *reinterpret_cast<std::uint64_t*>(&Instructions[i + 4]);
						i += 3 + 8;
						break;
					default:
						throw std::exception("invalid byte count");
					}
					break;
				case 1:
					Registers[Instructions[i + 2]] /= Registers[Instructions[i + 3]];
					i += 2;
					break;
				default:
					throw std::exception("invalid mode");
				}
				break;

			case AND:
				/*
					arguments:
					Is_register = 1/0
					Type = 1/2/4/32/64
				*/
				switch (Instructions[i + 1])
				{
				case 0:
					size_per_byte = Instructions[i + 2];
					switch (size_per_byte)
					{
					case 1:
						*reinterpret_cast<std::uint8_t*>(&Registers[Instructions[i + 3]]) &= *reinterpret_cast<std::uint8_t*>(&Instructions[i + 4]);
						i += 3 + 1;
						break;
					case 2:
						*reinterpret_cast<std::uint16_t*>(&Registers[Instructions[i + 3]]) &= *reinterpret_cast<std::uint16_t*>(&Instructions[i + 4]);
						i += 3 + 2;
						break;
					case 4:
						*reinterpret_cast<std::uint32_t*>(&Registers[Instructions[i + 3]]) &= *reinterpret_cast<std::uint32_t*>(&Instructions[i + 4]);
						i += 3 + 4;
						break;
					case 8:
						*reinterpret_cast<std::uint64_t*>(&Registers[Instructions[i + 3]]) &= *reinterpret_cast<std::uint64_t*>(&Instructions[i + 4]);
						i += 3 + 8;
						break;
					default:
						throw std::exception("invalid byte count");
					}
					break;
				case 1:
					Registers[Instructions[i + 2]] &= Registers[Instructions[i + 3]];
					i += 2;
					break;
				default:
					throw std::exception("invalid mode");
				}
				break;
			case OR:
				/*
					arguments:
					Is_register = 1/0
					Type = 1/2/4/32/64
				*/
				switch (Instructions[i + 1])
				{
				case 0:
					size_per_byte = Instructions[i + 2];
					switch (size_per_byte)
					{
					case 1:
						*reinterpret_cast<std::uint8_t*>(&Registers[Instructions[i + 3]]) |= *reinterpret_cast<std::uint8_t*>(&Instructions[i + 4]);
						i += 3 + 1;
						break;
					case 2:
						*reinterpret_cast<std::uint16_t*>(&Registers[Instructions[i + 3]]) |= *reinterpret_cast<std::uint16_t*>(&Instructions[i + 4]);
						i += 3 + 2;
						break;
					case 4:
						*reinterpret_cast<std::uint32_t*>(&Registers[Instructions[i + 3]]) |= *reinterpret_cast<std::uint32_t*>(&Instructions[i + 4]);
						i += 3 + 4;
						break;
					case 8:
						*reinterpret_cast<std::uint64_t*>(&Registers[Instructions[i + 3]]) |= *reinterpret_cast<std::uint64_t*>(&Instructions[i + 4]);
						i += 3 + 8;
						break;
					default:
						throw std::exception("invalid byte count");
					}
					break;
				case 1:
					Registers[Instructions[i + 2]] |= Registers[Instructions[i + 3]];
					i += 2;
					break;
				default:
					throw std::exception("invalid mode");
				}
				break;
			case XOR:
				/*
					arguments:
					Is_register = 1/0
					Type = 1/2/4/32/64
				*/
				switch (Instructions[i + 1])
				{
				case 0:
					size_per_byte = Instructions[i + 2];
					switch (size_per_byte)
					{
					case 1:
						*reinterpret_cast<std::uint8_t*>(&Registers[Instructions[i + 3]]) ^= *reinterpret_cast<std::uint8_t*>(&Instructions[i + 4]);
						i += 3 + 1;
						break;
					case 2:
						*reinterpret_cast<std::uint16_t*>(&Registers[Instructions[i + 3]]) ^= *reinterpret_cast<std::uint16_t*>(&Instructions[i + 4]);
						i += 3 + 2;
						break;
					case 4:
						*reinterpret_cast<std::uint32_t*>(&Registers[Instructions[i + 3]]) ^= *reinterpret_cast<std::uint32_t*>(&Instructions[i + 4]);
						i += 3 + 4;
						break;
					case 8:
						*reinterpret_cast<std::uint64_t*>(&Registers[Instructions[i + 3]]) ^= *reinterpret_cast<std::uint64_t*>(&Instructions[i + 4]);
						i += 3 + 8;
						break;
					default:
						throw std::exception("invalid byte count");
					}
					break;
				case 1:
					Registers[Instructions[i + 2]] ^= Registers[Instructions[i + 3]];
					i += 2;
					break;
				default:
					throw std::exception("invalid mode");
				}
				break;
			case NOT:
				/*
					arguments:
					Is_register = 1/0
					Type = 1/2/4/32/64
				*/
				switch (Instructions[i + 1])
				{
				case 0:
					size_per_byte = Instructions[i + 2];
					switch (size_per_byte)
					{
					case 1:
						*reinterpret_cast<std::uint8_t*>(&Registers[Instructions[i + 3]]) = !*reinterpret_cast<std::uint8_t*>(&Instructions[i + 3]);
						i += 3 + 1;
						break;
					case 2:
						*reinterpret_cast<std::uint16_t*>(&Registers[Instructions[i + 3]]) = !*reinterpret_cast<std::uint16_t*>(&Instructions[i + 3]);
						i += 3 + 2;
						break;
					case 4:
						*reinterpret_cast<std::uint32_t*>(&Registers[Instructions[i + 3]]) = !*reinterpret_cast<std::uint32_t*>(&Instructions[i + 3]);
						i += 3 + 4;
						break;
					case 8:
						*reinterpret_cast<std::uint64_t*>(&Registers[Instructions[i + 3]]) = !*reinterpret_cast<std::uint64_t*>(&Instructions[i + 3]);
						i += 3 + 8;
						break;
					default:
						throw std::exception("invalid byte count");
					}
					break;
				case 1:
					Registers[Instructions[i + 2]] = !Registers[Instructions[i + 2]];
					i += 2;
					break;
				default:
					throw std::exception("invalid mode");
				}
				break;
			case INC:
				/*
					arguments:
					Register
				*/
				Registers[Instructions[i + 1]]++;
				break;
			case DEC:
				/*
					arguments:
					Register
				*/
				Registers[Instructions[i + 1]]--;
				break;
			case PUSH:
				/*
					arguments:
					Is_register = 1/0
					Type = 1/2/4/32/64
					data = xxx
				*/
				switch (Instructions[i + 1])
				{
				case 0:
					size_per_byte = Instructions[i + 2];
					switch (size_per_byte)
					{
					case 1:
						Stack[++StackCounter] = *reinterpret_cast<std::uint8_t*>(&Instructions[i + 3]);
						i += 2 + 1;
						break;
					case 2:
						Stack[++StackCounter] = *reinterpret_cast<std::uint16_t*>(&Instructions[i + 3]);
						i += 2 + 2;
						break;
					case 4:
						Stack[++StackCounter] = *reinterpret_cast<std::uint32_t*>(&Instructions[i + 3]);
						i += 2 + 4;
						break;
					case 8:
						Stack[++StackCounter] = *reinterpret_cast<std::uint64_t*>(&Instructions[i + 3]);
						i += 2 + 8;
						break;
					default:
						throw std::exception("invalid byte count");
					}
					break;
				case 1:
					Stack[++StackCounter] = Registers[Instructions[i + 2]];
					i += 1;
					break;
				default:
					throw std::exception("invalid mode");
				}
				break;
			case POP:
				/*
					arguments:
					Is_register = 1/0
					Type = 1/2/4/32/64
				*/
				switch (Instructions[i + 1])
				{
				case 0:
					size_per_byte = Instructions[i + 2];
					switch (size_per_byte)
					{
					case 1:
						Stack[--StackCounter] = 0;
						i += 2 + 1;
						break;
					case 2:
						Stack[--StackCounter] = 0;
						i += 2 + 2;
						break;
					case 4:
						Stack[--StackCounter] = 0;
						i += 2 + 4;
						break;
					case 8:
						Stack[--StackCounter] = 0;
						i += 2 + 8;
						break;
					default:
						throw std::exception("invalid byte count");
					}
					break;
				case 1:
					for (int c = 0; c < StackCounter; c++)
					{
						if (Stack[c] == Instructions[i + 2])
						{
							Stack[c] = 0;
							break;
						}
					}
					i += 2;
					break;
				default:
					throw std::exception("invalid mode");
				}
				break;
			case LSTART:
				/*
					arguments:
					Is_register = 1/0
					function index
				*/
				size_per_byte = Instructions[i + 2];
				switch (Instructions[i + 1])
				{
				case 0:
					FunctionBytes.clear();
					Labels[Instructions[i + 2]] = i + 2;
					for (int in = 1; Instructions[in + i + 2] != LEND; in++)
						FunctionBytes.push_back(Instructions[i + in + 2]);
					i += FunctionBytes.size();
					break;
				case 1:
					FunctionBytes.clear();
					Labels[Registers[Instructions[i + 1]]] = i + 2;
					for (int in = 1; Instructions[in + i + 2] != LEND; in++)
						FunctionBytes.push_back(Instructions[i + in + 2]);
					i += FunctionBytes.size();
					break;
				default:
					throw std::exception("invalid mode");
				}
				break;
			case LEND:
				/*
					arguments:
					Is_register = 1/0
					Type = 1/2/4/32/64
				*/
				if (OldCounter)
				{
					i = OldCounter;
					OldCounter = 0;
				}
				break;
			case CALL:
				/*
					arguments:
					Is_register = 1/0
					index of function
				*/
				size_per_byte = Instructions[i + 2];
				switch (Instructions[i + 1])
				{
				case 0:
					OldCounter = i + 2;
					i = Labels[Instructions[i + 2]];
					continue;
					break;
				case 1:
					OldCounter = i + 2;
					i = Labels[Registers[Instructions[i + 2]]];
					continue;
					break;
				default:
					throw std::exception("invalid mode");
				}
				break;
			case RET:
				/*
					arguments:
					Is_register = 1/0
					index of function
				*/
				int counter;
				counter = 0;
				while (Instructions[i + counter] != LEND)
					counter++;
				i += counter-1;
				break;
			case MOV:
				/*
					arguments:
					Is_register_or_stack = 1/0
					Type = 1/2/4/32/64
				*/
				switch (Instructions[i + 1])
				{
				case 0:
					size_per_byte = Instructions[i + 2];
					switch (size_per_byte)
					{
					case 1:
						*reinterpret_cast<std::uint8_t*>(&Registers[Instructions[i + 3]]) = *reinterpret_cast<std::uint8_t*>(&Instructions[i + 4]);
						i += 3 + 1;
						break;
					case 2:
						*reinterpret_cast<std::uint16_t*>(&Registers[Instructions[i + 3]]) = *reinterpret_cast<std::uint16_t*>(&Instructions[i + 4]);
						i += 3 + 2;
						break;
					case 4:
						*reinterpret_cast<std::uint32_t*>(&Registers[Instructions[i + 3]]) = *reinterpret_cast<std::uint32_t*>(&Instructions[i + 4]);
						i += 3 + 4;
						break;
					case 8:
						*reinterpret_cast<std::uint64_t*>(&Registers[Instructions[i + 3]]) = *reinterpret_cast<std::uint64_t*>(&Instructions[i + 4]);
						i += 3 + 8;
						break;
					default:
						throw std::exception("invalid byte count");
					}
					break;
				case 1:
					Registers[Instructions[i + 2]] = Registers[Instructions[i + 2]];
					i += 2;
					break;
				case 2:
					size_per_byte = Instructions[i + 2];
					switch (size_per_byte)
					{
					case 1:
						*reinterpret_cast<std::uint8_t*>(&Registers[Instructions[i + 3]]) = *reinterpret_cast<std::uint8_t*>(&Stack[StackCounter]);
						i += 2 + 1;
						break;
					case 2:
						*reinterpret_cast<std::uint16_t*>(&Registers[Instructions[i + 3]]) = *reinterpret_cast<std::uint16_t*>(&Stack[StackCounter]);
						i += 2 + 2;
						break;
					case 4:
						*reinterpret_cast<std::uint32_t*>(&Registers[Instructions[i + 3]]) = *reinterpret_cast<std::uint32_t*>(&Stack[StackCounter]);
						i += 2 + 4;
						break;
					case 8:
						*reinterpret_cast<std::uint64_t*>(&Registers[Instructions[i + 3]]) = *reinterpret_cast<std::uint64_t*>(&Stack[StackCounter]);
						i += 2 + 8;
						break;
					default:
						throw std::exception("invalid byte count");
					}
					break;
				default:
					throw std::exception("invalid mode");
				}

				break;
			}
		}
	FINISH:
		return;
	}

	std::map<std::uintptr_t, std::uintptr_t> Labels;
	std::map<std::uintptr_t, std::uintptr_t> Registers;
	std::map<std::uintptr_t, std::uintptr_t> Flags;
	std::uint8_t *Stack;
	std::uint32_t OldCounter;
	std::uintptr_t StackCounter;
	std::uint32_t StackSize;
};