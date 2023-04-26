#include <stdio.h>
#include <malloc.h>
#include <windows.h>

#define i8 char

//struct HashSet
//{
//	char* _data;
//	int _maxLength;
//
//	HashSet()
//	{
//		_maxLength = 10;
//		_data = (char*)malloc(_maxLength * sizeof(int));
//	}
//
//	void Set(int key, int value)
//	{
//		key % _maxLength;
//	}
//};

struct Tuple
{
	int first;
	int second;
};

struct DynamicArray
{
	char* _data;
	int _maxElementsCount;
	int _latestElementIndex;
	int _singleElementSize;

	DynamicArray(int initMaxElementsCount, int singleElementSize)
	{
		_singleElementSize = singleElementSize;
		_latestElementIndex = 0;
		_maxElementsCount = initMaxElementsCount;
		_data = (char*)malloc(_singleElementSize * _maxElementsCount);
	}

	void add(char* elementToAdd)
	{
		memcpy(_data + _singleElementSize * _latestElementIndex, elementToAdd, _singleElementSize);
		_latestElementIndex++;

		if (_latestElementIndex >= _maxElementsCount)
		{
			_maxElementsCount *= 1.5;
			_data = (char*)realloc(_data, _singleElementSize * _maxElementsCount);
		}
	}

	void replaceByIndex(char* element, int index)
	{
		memcpy(_data + _singleElementSize * index, element, _singleElementSize);
	}
};

const char AL[] = "al";
const char AX[] = "ax";
const char CL[] = "cl";
const char CX[] = "cx";
const char DL[] = "dl";
const char DX[] = "dx";
const char BL[] = "bl";
const char BX[] = "bx";
const char AH[] = "ah";
const char SP[] = "sp";
const char CH[] = "ch";
const char BP[] = "bp";
const char DH[] = "dh";
const char SI[] = "si";
const char BH[] = "bh";
const char DI[] = "di";

char* InternalAppendToString(char* initString, char* stringToAppend)
{
	int newStringLength = strlen(initString) + strlen(stringToAppend);
	char* newString = (char*)malloc(newStringLength + 1);
	memcpy(newString, initString, strlen(initString));
	memcpy(newString + strlen(initString), stringToAppend, strlen(stringToAppend));

	newString[newStringLength] = '\0';

	return newString;
}

char* AppendToString(char* initString, char* stringToAppend)
{
	char* newStr = InternalAppendToString(initString, stringToAppend);
	free(stringToAppend);
	free(initString);
	return newStr;
}

char* AppendToString(char* initString, const char* stringToAppend)
{
	char* newStr = InternalAppendToString(initString, (char*)stringToAppend);
	free(initString);

	return newStr;
}

char* AppendToString(const char* initString, const char* stringToAppend)
{
	return InternalAppendToString((char*)initString, (char*)stringToAppend);
}

char* AppendToString(const char* initString, char* stringToAppend)
{
	char* newStr = InternalAppendToString((char*)initString, stringToAppend);
	free(stringToAppend);

	return newStr;
}

char* AllocateString(const char* strToAllocate)
{
	int strToAllocateLength = strlen(strToAllocate);
	char* str = (char*)malloc(strToAllocateLength + 1);
	memcpy(str, strToAllocate, strToAllocateLength);
	str[strToAllocateLength] = '\0';
	return str;
}

char* AllocateEmptyString()
{
	char* str = (char*)malloc(1);
	str[0] = '\0';
	return str;
}

char* InternalWrapStringWithSymbols(char* originalString, char startSymbol, char endSymbol)
{
	int wrappedStringLength = 2 + strlen(originalString);
	char* wrappedString = (char*)malloc((1 + wrappedStringLength) * sizeof(char));

	wrappedString[0] = startSymbol;
	for (int i = 0; i < strlen(originalString); i++)
	{
		wrappedString[i + 1] = originalString[i];
	}
	wrappedString[wrappedStringLength - 1] = endSymbol;
	wrappedString[wrappedStringLength] = '\0';

	return wrappedString;
}

char* WrapStringWithSymbols(char* originalString, char startSymbol, char endSymbol)
{
	char* newStr = InternalWrapStringWithSymbols(originalString, startSymbol, endSymbol);

	free(originalString);
	return newStr;
}

char* WrapStringWithSymbols(const char* originalString, char startSymbol, char endSymbol)
{
	char* newStr = InternalWrapStringWithSymbols((char*)originalString, startSymbol, endSymbol);

	return newStr;
}

const char* DecodeEffectiveAddressFormulas(char rmField)
{
	if ((rmField ^ 0b000) == 0)
	{
		return "bx + si";
	}

	if ((rmField ^ 0b001) == 0)
	{
		return "bx + di";
	}

	if ((rmField ^ 0b010) == 0)
	{
		return "bp + si";
	}

	if ((rmField ^ 0b011) == 0)
	{
		return "bp + di";
	}

	if ((rmField ^ 0b100) == 0)
	{
		return "si";
	}

	if ((rmField ^ 0b101) == 0)
	{
		return "di";
	}

	if ((rmField ^ 0b110) == 0)
	{
		return "bp";
	}

	if ((rmField ^ 0b111) == 0)
	{
		return "bx";
	}
}

const char* DecodeRegisterName(char registerBinary, bool isWide)
{
	if ((registerBinary ^ 0b000) == 0)
	{
		return isWide ? AX : AL;
	}
	else if ((registerBinary ^ 0b001) == 0)
	{
		return isWide ? CX : CL;
	}
	else if ((registerBinary ^ 0b010) == 0)
	{
		return isWide ? DX : DL;
	}
	else if ((registerBinary ^ 0b011) == 0)
	{
		return isWide ? BX : BL;
	}
	else if ((registerBinary ^ 0b100) == 0)
	{
		return isWide ? SP : AH;
	}
	else if ((registerBinary ^ 0b101) == 0)
	{
		return isWide ? BP : CH;
	}
	else if ((registerBinary ^ 0b110) == 0)
	{
		return isWide ? SI : DH;
	}
	else if ((registerBinary ^ 0b111) == 0)
	{
		return isWide ? DI : BH;
	}

	return "";
}

char* WriteDecimalNumberToBuffer(int valueToWrite)
{
	int iterator = valueToWrite;
	int numberLength = 0;

	bool isNegative = valueToWrite < 0;

	while (iterator != 0)
	{
		numberLength++;
		iterator /= 10;
	}

	int numberStringLength = numberLength + (isNegative ? 1 : 0);

	if (valueToWrite == 0)
	{
		numberStringLength = 1;
	}

	char* strNumber = (char*)malloc((numberStringLength + 1) * sizeof(char));

	if (valueToWrite == 0)
	{
		strNumber[0] = '0';
		strNumber[1] = '\0';

		return strNumber;
	}

	valueToWrite = valueToWrite < 0 ? -1 * valueToWrite : valueToWrite;

	for (int i = 0; i < numberLength; i++)
	{
		int symboltoWrite = valueToWrite % 10;

		strNumber[numberLength + (isNegative ? 1 : 0) - i - 1] = 48 + symboltoWrite;

		valueToWrite /= 10;
	}

	if (isNegative)
	{
		strNumber[0] = '-';
	}

	strNumber[numberStringLength] = '\0';

	return strNumber;
}

char* ImmediateToRegOrMemoryCommand(
	char* command,
	int* currentByteIndex,
	char* instructionsBuffer,
	bool hasSigned)
{
	unsigned char firstByte = instructionsBuffer[*currentByteIndex];
	unsigned char secondByte = instructionsBuffer[*currentByteIndex + 1];

	char* leftPart = AllocateEmptyString();
	char* rightPart = AllocateEmptyString();

	bool isWide = (firstByte & 0b00000001) == 0b00000001;
	char modPart = (secondByte & 0b11000000) >> 6;
	char rmPart = (secondByte & 0b00000111);

	if (modPart == 0b11) // to register
	{
		rightPart = AppendToString(rightPart, DecodeRegisterName(rmPart, isWide));
		*currentByteIndex += 2;
	}
	else if (modPart == 0b00) // effective address (no displacement)
	{
		if (rmPart == 0b110) // direct memory
		{
			int directAddress = *(short*)(instructionsBuffer + *currentByteIndex + 2);
			char* directAddressStr = WriteDecimalNumberToBuffer(directAddress);

			rightPart = WrapStringWithSymbols(directAddressStr, '[', ']');
			*currentByteIndex += 4;
		}
		else
		{
			const char* effectiveAddressFormula = DecodeEffectiveAddressFormulas(rmPart);
			rightPart = WrapStringWithSymbols(effectiveAddressFormula, '[', ']');
			*currentByteIndex += 2;
		}
	}
	else if (modPart == 0b01) // effective address (8 bit)
	{
		const char* effectiveAddressFormula = DecodeEffectiveAddressFormulas(rmPart);

		char displacementValue = *(char*)(instructionsBuffer + *currentByteIndex + 2);
		char* strNumber = WriteDecimalNumberToBuffer((int)abs(displacementValue));

		char* eAF1 = AppendToString(effectiveAddressFormula, displacementValue < 0 ? " - " : " + ");
		char* eAF2 = AppendToString(eAF1, strNumber);
		rightPart = WrapStringWithSymbols(eAF2, '[', ']');

		*currentByteIndex += 1;
	}
	else if (modPart == 0b10) // effective address (16 bit)
	{
		const char* effectiveAddressFormula = DecodeEffectiveAddressFormulas(rmPart);

		short displacementValue = *(short*)(instructionsBuffer + *currentByteIndex + 2);
		char* strNumber = WriteDecimalNumberToBuffer((int)abs(displacementValue));

		char* eAF1 = AppendToString(effectiveAddressFormula, displacementValue < 0 ? " - " : " + ");
		char* eAF2 = AppendToString(eAF1, strNumber);
		rightPart = WrapStringWithSymbols(eAF2, '[', ']');

		*currentByteIndex += 4;
	}

	if (modPart == 0b00 || modPart == 0b01 || modPart == 0b10)
	{
		leftPart = isWide ? AppendToString(leftPart, "word ") : AppendToString(leftPart, "byte ");
	}

	if (hasSigned)
	{
		bool isSigned = (firstByte & 0b00000010) == 0b00000010;

		int immediateValue = (!isSigned && isWide)
			? *(short*)(instructionsBuffer + *currentByteIndex)
			: *(char*)(instructionsBuffer + *currentByteIndex);

		char* immediateValueStr = WriteDecimalNumberToBuffer(immediateValue);

		*currentByteIndex += (!isSigned && isWide) ? 2 : 1;
		leftPart = AppendToString(leftPart, immediateValueStr);
	}
	else
	{
		int immediateValue = 0;
		memcpy(&immediateValue, instructionsBuffer + *currentByteIndex, isWide ? 2 : 1);
		char* immediateValueStr = WriteDecimalNumberToBuffer((int)abs(immediateValue));
		*currentByteIndex += isWide ? 2 : 1;
		leftPart = AppendToString(leftPart, immediateValueStr);
	}

	command = AppendToString(command, rightPart);
	command = AppendToString(command, ", ");
	command = AppendToString(command, leftPart);

	return command;
}

char* RegMemoryCommand(char* command, int* currentByteIndex, char* instructionsBuffer)
{
	unsigned char firstByte = instructionsBuffer[*currentByteIndex];
	unsigned char secondByte = instructionsBuffer[*currentByteIndex + 1];

	char* destinationPart = AllocateEmptyString();
	char* sourcePart = AllocateEmptyString();

	bool isDestinationSwitched = (firstByte & 0b00000010) == 0b00000010;
	bool isWide = (firstByte & 0b00000001) == 0b00000001;

	char modFlag = (secondByte & 0b11000000) >> 6;
	char regFlag = (secondByte & 0b00111000) >> 3;
	char rmFlag = secondByte & 0b00000111;

	sourcePart = AppendToString(sourcePart, DecodeRegisterName(regFlag, isWide));
	switch (modFlag)
	{
	case 0b00:
	{
		if (rmFlag == 0b110)
		{
			short immediateValue = *(short*)(instructionsBuffer + *currentByteIndex + 2);
			char* strNumber = WriteDecimalNumberToBuffer((int)abs(immediateValue));
			destinationPart = AppendToString(destinationPart, WrapStringWithSymbols(strNumber, '[', ']'));

			*currentByteIndex += 4;
		}
		else
		{
			const char* effectiveAddressFormula = DecodeEffectiveAddressFormulas(rmFlag);

			destinationPart = AppendToString(destinationPart, WrapStringWithSymbols(effectiveAddressFormula, '[', ']'));
			*currentByteIndex += 2;
		}
		break;
	}
	case 0b01:
	{
		const char* effectiveAddressFormula = DecodeEffectiveAddressFormulas(rmFlag);
		int formulaLength = strlen(effectiveAddressFormula);

		char immediateValue = *(instructionsBuffer + *currentByteIndex + 2);
		char* strNumber = WriteDecimalNumberToBuffer((int)abs(immediateValue));

		char* eAF1 = AppendToString(effectiveAddressFormula, immediateValue < 0 ? " - " : " + ");
		char* eAF2 = AppendToString(eAF1, strNumber);
		destinationPart = AppendToString(destinationPart, WrapStringWithSymbols(eAF2, '[', ']'));

		*currentByteIndex += 3;
		break;
	}
	case 0b10:
	{
		const char* effectiveAddressFormula = DecodeEffectiveAddressFormulas(rmFlag);
		int formulaLength = strlen(effectiveAddressFormula);

		short immediateValue = *(short*)(instructionsBuffer + *currentByteIndex + 2);
		char* strNumber = WriteDecimalNumberToBuffer((int)abs(immediateValue));

		char* eAF1 = AppendToString(effectiveAddressFormula, immediateValue < 0 ? " - " : " + ");
		char* eAF2 = AppendToString(eAF1, strNumber);
		destinationPart = AppendToString(destinationPart, WrapStringWithSymbols(eAF2, '[', ']'));

		*currentByteIndex += 4;
		break;
	}
	case 0b11:
	{
		destinationPart = AppendToString(destinationPart, DecodeRegisterName(rmFlag, isWide));

		*currentByteIndex += 2;
		break;
	}
	}

	if (isDestinationSwitched)
	{
		char* tmpStr;
		tmpStr = destinationPart;
		destinationPart = sourcePart;
		sourcePart = tmpStr;
	}

	command = AppendToString(command, destinationPart);
	command = AppendToString(command, ", ");
	command = AppendToString(command, sourcePart);

	return command;
}

char* ImmediateToRegCommand(char* command, int* currentByteIndex, char* instructionsBuffer)
{
	unsigned char firstByte = instructionsBuffer[*currentByteIndex];
	unsigned char secondByte = instructionsBuffer[*currentByteIndex + 1];

	bool isWide = (firstByte & 0b00001000) == 0b00001000;
	char regFlag = firstByte & 0b00000111;

	const char* registerName = DecodeRegisterName(regFlag, isWide);

	int immediateValue = isWide
		? *(short*)(instructionsBuffer + *currentByteIndex + 1)
		: *(char*)(instructionsBuffer + *currentByteIndex + 1);
	char* immediateValueStr = WriteDecimalNumberToBuffer((int)immediateValue);
	*currentByteIndex += isWide ? 3 : 2;

	command = AppendToString(command, registerName);
	command = AppendToString(command, ", ");
	command = AppendToString(command, immediateValueStr);

	return command;
}

char* ImmediateToAccumulatorCommand(char* command, int* currentByteIndex, char* instructionsBuffer)
{
	unsigned char firstByte = instructionsBuffer[*currentByteIndex];
	unsigned char secondByte = instructionsBuffer[*currentByteIndex + 1];

	bool isWide = (firstByte & 0b00000001) == 0b00000001;

	const char* registerName = isWide ? "ax" : "al";

	int immediateValue = isWide
		? *(short*)(instructionsBuffer + *currentByteIndex + 1)
		: *(char*)(instructionsBuffer + *currentByteIndex + 1);
	char* immediateValueStr = WriteDecimalNumberToBuffer(immediateValue);
	*currentByteIndex += isWide ? 3 : 2;

	command = AppendToString(command, registerName);
	command = AppendToString(command, ", ");
	command = AppendToString(command, immediateValueStr);

	return command;
}

char* AddStringToBuffer(
	char* buffer,
	char* strToAppend,
	int* currentBufferIndex,
	int* maxBufferSize)
{
	int strSize = strlen(strToAppend) * sizeof(char);
	*currentBufferIndex += strSize;

	while (*currentBufferIndex >= *maxBufferSize)
	{
		*maxBufferSize *= 1.5;
		buffer = (char*)realloc(buffer, *maxBufferSize);
	}

	memcpy(buffer + *currentBufferIndex - strSize, strToAppend, strlen(strToAppend) * sizeof(char));

	return buffer;
}

void InsertStringIntoBuffer(int indexToInsert,
	char* stringToInsert,
	char** buffer,
	int* bufferSize)
{
	int stringSize = strlen(stringToInsert);
	char* newBuffer = (char*)malloc(*bufferSize + stringSize);

	memcpy(newBuffer, *buffer, indexToInsert);
	memcpy(newBuffer + indexToInsert, stringToInsert, stringSize);
	memcpy(newBuffer + indexToInsert + stringSize, *buffer + indexToInsert,
		*bufferSize - indexToInsert);

	*bufferSize += stringSize;
	free(*buffer);
	*buffer = newBuffer;
}

void main()
{
	FILE* asmFile;
	int err = fopen_s(&asmFile, "./original_asm", "rb");
	fseek(asmFile, 0L, SEEK_END);
	int fileLength = ftell(asmFile);

	char* instructionsBuffer = (char*)malloc(fileLength);
	fseek(asmFile, 0L, SEEK_SET);

	fread(instructionsBuffer, fileLength, 1, asmFile);
	fclose(asmFile);

	FILE* decodedFile;
	fopen_s(&decodedFile, "decoded_asm.asm", "w+");

	int maxDecodedFileBufferSize = 2;
	DynamicArray decodedCommands = DynamicArray(10, sizeof(char*));
	DynamicArray commandsAddresses = DynamicArray(10, sizeof(int));

	// TODO: probably it's better to replace it by dynamic array
	int addressesToJump[100];
	int latestAddedAddressIndex = 0;
	for (int i = 0; i < 100; i++) addressesToJump[i] = -1;

	// opcodes
	char movOpcode = 0b100010;
	char immediateToRegistryMovOpcode = 0b1011;
	char immediateToRegistryMemoryMovOpcode = 0b1100011;
	char accumulatorToMemoryMovOpcode = 0b1010001;
	char memoryToAccumulatorMovOpcode = 0b1010000;

	char addRegMemoryWithRegisterToEither = 0b000000;
	char addImmediateToRegisterOrMemory = 0b100000;
	char addImmediateToAccumulator = 0b0000010;

	char subRegMemoryWithRegisterToEither = 0b001010;
	char subImmediateToRegisterOrMemory = 0b100000;
	char subImmediateToAccumulator = 0b0010110;

	char cmpRegMemoryWithRegisterToEither = 0b001110;
	char cmpImmediateToRegisterOrMemory = 0b100000;
	char cmpImmediateToAccumulator = 0b0011110;

	char jumpOnNotEqual = 0b01110101;

	int byteIndex = 0;
	DynamicArray jumpAddressMap = DynamicArray(10, sizeof(Tuple));

	while (byteIndex < fileLength)
	{
		commandsAddresses.add((char*)&byteIndex);

		unsigned char firstByte = instructionsBuffer[byteIndex];
		unsigned char secondByte = instructionsBuffer[byteIndex + 1];

		char regFlag = (secondByte & 0b00111000) >> 3;

		char* command = AllocateEmptyString();
		// add
		if (addRegMemoryWithRegisterToEither == (firstByte >> 2))
		{
			command = AppendToString(command, "add ");
			command = RegMemoryCommand(command, &byteIndex, instructionsBuffer);
		}
		else if (addImmediateToRegisterOrMemory == (firstByte >> 2) && (regFlag == 0b000))
		{
			command = AppendToString(command, "add ");
			command = ImmediateToRegOrMemoryCommand(command, &byteIndex, instructionsBuffer, true);
		}
		else if (addImmediateToAccumulator == (firstByte >> 1))
		{
			command = AppendToString(command, "add ");
			command = ImmediateToAccumulatorCommand(command, &byteIndex, instructionsBuffer);
		}
		// sub
		else if (subRegMemoryWithRegisterToEither == (firstByte >> 2))
		{
			command = AppendToString(command, "sub ");
			command = RegMemoryCommand(command, &byteIndex, instructionsBuffer);
		}
		else if (subImmediateToRegisterOrMemory == (firstByte >> 2) && (regFlag == 0b101))
		{
			command = AppendToString(command, "sub ");
			command = ImmediateToRegOrMemoryCommand(command, &byteIndex, instructionsBuffer, true);
		}
		else if (subImmediateToAccumulator == (firstByte >> 1))
		{
			command = AppendToString(command, "sub ");
			command = ImmediateToAccumulatorCommand(command, &byteIndex, instructionsBuffer);
		}
		// cmp
		else if (cmpRegMemoryWithRegisterToEither == (firstByte >> 2))
		{
			command = AppendToString(command, "cmp ");
			command = RegMemoryCommand(command, &byteIndex, instructionsBuffer);
		}
		else if (cmpImmediateToRegisterOrMemory == (firstByte >> 2) && (regFlag == 0b111))
		{
			command = AppendToString(command, "cmp ");
			command = ImmediateToRegOrMemoryCommand(command, &byteIndex, instructionsBuffer, true);
		}
		else if (cmpImmediateToAccumulator == (firstByte >> 1))
		{
			command = AppendToString(command, "cmp ");
			command = ImmediateToAccumulatorCommand(command, &byteIndex, instructionsBuffer);
		}
		// mov
		else if (movOpcode == (firstByte >> 2))
		{
			command = AppendToString(command, "mov ");
			command = RegMemoryCommand(command, &byteIndex, instructionsBuffer);
		}
		else if ((immediateToRegistryMovOpcode ^ (unsigned char)(firstByte >> 4)) == 0)
		{
			command = AppendToString(command, "mov ");
			command = ImmediateToRegCommand(command, &byteIndex, instructionsBuffer);
		}
		else if ((immediateToRegistryMemoryMovOpcode ^ (unsigned char)(firstByte >> 1)) == 0)
		{
			command = AppendToString(command, "mov ");
			command = ImmediateToRegOrMemoryCommand(command, &byteIndex, instructionsBuffer, false);
		}
		else if (memoryToAccumulatorMovOpcode == (char)(firstByte >> 1))
		{
			command = AppendToString(command, "mov ");

			bool isWide = (firstByte & 0b00000001) == 0b00000001;

			const char* registerName = "ax";

			int immediateValue = isWide
				? *(short*)(instructionsBuffer + byteIndex + 1)
				: *(char*)(instructionsBuffer + byteIndex + 1);
			char* immediateValueStr = WriteDecimalNumberToBuffer((int)immediateValue);
			immediateValueStr = WrapStringWithSymbols(immediateValueStr, '[', ']');

			byteIndex += isWide ? 3 : 2;

			command = AppendToString(command, registerName);
			command = AppendToString(command, ", ");
			command = AppendToString(command, immediateValueStr);
		}
		else if (accumulatorToMemoryMovOpcode == (char)(firstByte >> 1))
		{
			command = AppendToString(command, "mov ");

			bool isWide = (firstByte & 0b00000001) == 0b00000001;

			const char* registerName = "ax";

			int immediateValue = isWide
				? *(short*)(instructionsBuffer + byteIndex + 1)
				: *(char*)(instructionsBuffer + byteIndex + 1);
			char* immediateValueStr = WriteDecimalNumberToBuffer((int)immediateValue);
			immediateValueStr = WrapStringWithSymbols(immediateValueStr, '[', ']');

			byteIndex += isWide ? 3 : 2;

			command = AppendToString(command, immediateValueStr);
			command = AppendToString(command, ", ");
			command = AppendToString(command, registerName);
		}
		else if (jumpOnNotEqual == firstByte)
		{
			char secondByte = instructionsBuffer[byteIndex + 1];
			byteIndex += 2;
			int addressToJump = byteIndex + secondByte;

			//> Was address already added
			bool addressAlreadyAdded = false;
			for (int i = 0; i <= latestAddedAddressIndex; i++)
			{
				if (addressesToJump[i] == addressToJump)
				{
					addressAlreadyAdded = true;
					break;
				}
			}

			if (!addressAlreadyAdded)
			{
				addressesToJump[latestAddedAddressIndex] = addressToJump;
				latestAddedAddressIndex++;
			}
			//<

			//> sort Addresses
			for (int i = 0; i < latestAddedAddressIndex; i++)
			{
				for (int j = i; j < latestAddedAddressIndex; j++)
				{
					if (addressesToJump[j] < addressesToJump[i])
					{
						int tmp = addressesToJump[i];
						addressesToJump[i] = addressesToJump[j];
						addressesToJump[j] = tmp;
					}
				}
			}
			//<

			//> find index of address after it was sorted
			int addressIndex = -1;
			for (int i = 0; i < latestAddedAddressIndex; i++)
			{
				if (addressesToJump[i] == addressToJump)
				{
					addressIndex = i;
					break;
				}
			}
			//<

			command = AppendToString(command, "jnz");

			Tuple tuple = { decodedCommands._latestElementIndex, addressToJump };
			jumpAddressMap.add((char*)&tuple);
		}
		else
		{
			// unrecognized

			byteIndex += 2;
		}

		decodedCommands.add((char*)&command);
	}

	// add labels to jump instructions
	for (int i = 0; i < jumpAddressMap._latestElementIndex; i++)
	{
		char* labelName = AllocateEmptyString();
		labelName = AppendToString(labelName, " test_label");

		Tuple jumpAddress = ((Tuple*)jumpAddressMap._data)[i];
		int jumpCommandIndex = jumpAddress.first;
		int jumpToAddress = jumpAddress.second;

		for (int j = 0; j < 100; j++)
		{
			if (jumpToAddress == addressesToJump[j])
			{
				labelName = AppendToString(labelName, WriteDecimalNumberToBuffer(j));
				break;
			}
		}

		char* jumpCommand = ((char**)decodedCommands._data)[jumpCommandIndex];
		jumpCommand = AppendToString(jumpCommand, labelName);

		decodedCommands.replaceByIndex((char*)&jumpCommand, jumpCommandIndex);
	}

	// write to decoded file
	const char* fileHeader = "bits 16\n\n";
	fwrite(fileHeader, strlen(fileHeader), 1, decodedFile);

	int decodedFileIndex = 0;
	const char newLine = '\n';
	int addressesToJumpIndex = 0;
	for (int i = 0; i < decodedCommands._latestElementIndex; i++)
	{
		// labels writing
		int commandAddress = ((int*)commandsAddresses._data)[i];
		if (commandAddress == addressesToJump[addressesToJumpIndex])
		{
			char* labdelStr = AllocateString("test_label");
			labdelStr = AppendToString(labdelStr, WriteDecimalNumberToBuffer(addressesToJumpIndex));
			labdelStr = AppendToString(labdelStr, ":\n");

			fwrite(labdelStr, strlen(labdelStr), 1, decodedFile);
			addressesToJumpIndex++;
			free(labdelStr);
		}

		char* commandStr = ((char**)decodedCommands._data)[i];

		fwrite(commandStr, strlen(commandStr), 1, decodedFile);
		free(commandStr);

		fwrite(&newLine, 1, 1, decodedFile);
	}

	fseek(decodedFile, 0L, SEEK_END);
	fileLength = ftell(decodedFile);
	char* decodedBuffer = (char*)malloc(fileLength + 1);
	fseek(decodedFile, 0L, SEEK_SET);
	fread(decodedBuffer, fileLength, 1, decodedFile);
	decodedBuffer[fileLength] = '\0';
	printf(decodedBuffer);

	fclose(decodedFile);
}