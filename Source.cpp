#include <stdio.h>
#include <malloc.h>
#include <windows.h>

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

void main()
{
	const char commaSpace[] = ", ";

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

	const char fileHeader[] = "bits 16\n\n";
	fwrite(&fileHeader, strlen(fileHeader) * sizeof(char), 1, decodedFile);

	int byteIndex = 0;
	while (byteIndex < fileLength)
	{
		unsigned char firstByte = instructionsBuffer[byteIndex];
		unsigned char secondByte = instructionsBuffer[byteIndex + 1];

		// opcodes
		char movOpcode = 0b100010;
		char immediateToRegistryMovOpcode = 0b1011;
		char immediateToRegistryMemoryMovOpcode = 0b1100011;
		char accumulatorToMemoryMovOpcode = 0b1010001;
		char memoryToAccumulatorMovOpcode = 0b1010000;

		if ((movOpcode ^ (unsigned char)(firstByte >> 2)) == 0)
		{
			const char movOpName[] = "mov ";
			fwrite(&movOpName, strlen(movOpName) * sizeof(char), 1, decodedFile);
			char* leftPart = AllocateEmptyString();
			char* rightPart = AllocateEmptyString();

			bool isWide = ((firstByte & 0b00000001) ^ 0b00000001) == 0;
			bool isDestinationSwiched = ((firstByte & 0b00000010) ^ 0b00000010) == 0;

			// if it's store to RAM from register "destination" should be 0
			char modPart = secondByte & 0b11000000;
			// mod field tells whatever we have dress displacement in source or destination

			char regFlag = (secondByte & 0b00111000) >> 3;
			char rmFlag = secondByte & 0b00000111;
			// rm holds the "equation" part for effective address calculation if present

			leftPart = AppendToString(leftPart, DecodeRegisterName(regFlag, isWide));
			if (modPart == (char)0b11000000) // register to register
			{
				rightPart = AppendToString(rightPart, DecodeRegisterName(rmFlag, isWide));

				byteIndex += 2;
			}
			else if (modPart == (char)0b00000000) // no displacement
			{
				if (rmFlag == 0b110) // rm = 110, then 16-bit displacement follows (direct address mode)
				{
					short immediateValue = *(short*)(instructionsBuffer + byteIndex + 2);
					char* strNumber = WriteDecimalNumberToBuffer((int)abs(immediateValue));
					rightPart = AppendToString(rightPart, WrapStringWithSymbols(strNumber, '[', ']'));

					byteIndex += 4;
				}
				else
				{
					const char* effectiveAddressFormula = DecodeEffectiveAddressFormulas(rmFlag);

					rightPart = AppendToString(rightPart, WrapStringWithSymbols(effectiveAddressFormula, '[', ']'));
					byteIndex += 2;
				}
			}
			else if (modPart == (char)0b01000000) // 8 bit displacement
			{
				const char* effectiveAddressFormula = DecodeEffectiveAddressFormulas(rmFlag);
				int formulaLength = strlen(effectiveAddressFormula);

				char immediateValue = *(instructionsBuffer + byteIndex + 2);
				char* strNumber = WriteDecimalNumberToBuffer((int)abs(immediateValue));

				char* eAF1 = AppendToString(effectiveAddressFormula, immediateValue < 0 ? " - " : " + ");
				char* eAF2 = AppendToString(eAF1, strNumber);
				rightPart = AppendToString(rightPart, WrapStringWithSymbols(eAF2, '[', ']'));

				byteIndex += 3;
			}
			else if (modPart == (char)0b10000000) // 16 bit displacement
			{
				const char* effectiveAddressFormula = DecodeEffectiveAddressFormulas(rmFlag);
				int formulaLength = strlen(effectiveAddressFormula);

				short immediateValue = *(short*)(instructionsBuffer + byteIndex + 2);
				char* strNumber = WriteDecimalNumberToBuffer((int)abs(immediateValue));

				char* eAF1 = AppendToString(effectiveAddressFormula, immediateValue < 0 ? " - " : " + ");
				char* eAF2 = AppendToString(eAF1, strNumber);
				rightPart = AppendToString(rightPart, WrapStringWithSymbols(eAF2, '[', ']'));

				byteIndex += 4;
			}

			if (isDestinationSwiched)
			{
				char* tmpStr;
				tmpStr = leftPart;
				leftPart = rightPart;
				rightPart = tmpStr;
			}

			fwrite(rightPart, strlen(rightPart), 1, decodedFile);
			fwrite(&commaSpace, strlen(commaSpace) * sizeof(char), 1, decodedFile);
			fwrite(leftPart, strlen(leftPart), 1, decodedFile);
			free(leftPart);
			free(rightPart);
		}
		else if ((immediateToRegistryMovOpcode ^ (unsigned char)(firstByte >> 4)) == 0)
		{
			const char movOpName[] = "mov ";
			fwrite(&movOpName, strlen(movOpName) * sizeof(char), 1, decodedFile);

			bool isWide = (firstByte & 0b00001000) == 0b00001000;
			char regFlag = firstByte & 0b00000111;

			const char* registerName = DecodeRegisterName(regFlag, isWide);

			int immediateValue = isWide
				? *(short*)(instructionsBuffer + byteIndex + 1)
				: *(char*)(instructionsBuffer + byteIndex + 1);
			char* immediateValueStr = WriteDecimalNumberToBuffer((int)immediateValue);
			byteIndex += isWide ? 3 : 2;

			fwrite(registerName, 2 * sizeof(char), 1, decodedFile);
			fwrite(&commaSpace, strlen(commaSpace) * sizeof(char), 1, decodedFile);
			fwrite(immediateValueStr, strlen(immediateValueStr), 1, decodedFile);
		}
		else if ((immediateToRegistryMemoryMovOpcode ^ (unsigned char)(firstByte >> 1)) == 0)
		{
			char* leftPart = AllocateEmptyString();
			char* rightPart = AllocateEmptyString();
			const char movOpName[] = "mov ";
			fwrite(&movOpName, strlen(movOpName) * sizeof(char), 1, decodedFile);

			bool isWide = (firstByte & 0b00000001) == 0b00000001;
			char modPart = (secondByte & 0b11000000) >> 6;
			char rmPart = (secondByte & 0b00000111);

			if (modPart == 0b11) // to register
			{
				printf("");

			}
			else if (modPart == 0b00) // effective address (no displacement)
			{
				const char* effectiveAddressFormula = DecodeEffectiveAddressFormulas(rmPart);
				rightPart = WrapStringWithSymbols(effectiveAddressFormula, '[', ']');
				byteIndex += 2;
			}
			else if (modPart == 0b01) // effective address (8 bit)
			{
				const char* effectiveAddressFormula = DecodeEffectiveAddressFormulas(rmPart);

				char displacementValue = *(char*)(instructionsBuffer + byteIndex + 2);
				char* strNumber = WriteDecimalNumberToBuffer((int)abs(displacementValue));

				char* eAF1 = AppendToString(effectiveAddressFormula, " + ");
				char* eAF2 = AppendToString(eAF1, strNumber);
				rightPart = WrapStringWithSymbols(eAF2, '[', ']');

				byteIndex += 1;
			}
			else if (modPart == 0b10) // effective address (16 bit)
			{
				const char* effectiveAddressFormula = DecodeEffectiveAddressFormulas(rmPart);

				short displacementValue = *(short*)(instructionsBuffer + byteIndex + 2);
				char* strNumber = WriteDecimalNumberToBuffer((int)abs(displacementValue));

				char* eAF1 = AppendToString(effectiveAddressFormula, " + ");
				char* eAF2 = AppendToString(eAF1, strNumber);
				rightPart = WrapStringWithSymbols(eAF2, '[', ']');

				byteIndex += 4;
			}

			leftPart = isWide ? AppendToString(leftPart, "word ") : AppendToString(leftPart, "byte ");

			int immediateValue = 0;
			memcpy(&immediateValue, instructionsBuffer + byteIndex, isWide ? 2 : 1);
			char* immediateValueStr = WriteDecimalNumberToBuffer((int)abs(immediateValue));
			byteIndex += isWide ? 2 : 1;

			leftPart = AppendToString(leftPart, immediateValueStr);

			fwrite(rightPart, strlen(rightPart), 1, decodedFile);
			fwrite(&commaSpace, strlen(commaSpace) * sizeof(char), 1, decodedFile);
			fwrite(leftPart, strlen(leftPart), 1, decodedFile);
			free(leftPart);
			free(rightPart);
		}
		else if (memoryToAccumulatorMovOpcode == (char)(firstByte >> 1))
		{
			const char movOpName[] = "mov ";
			fwrite(&movOpName, strlen(movOpName) * sizeof(char), 1, decodedFile);

			bool isWide = (firstByte & 0b00000001) == 0b00000001;

			const char* registerName = "ax";

			int immediateValue = isWide
				? *(short*)(instructionsBuffer + byteIndex + 1)
				: *(char*)(instructionsBuffer + byteIndex + 1);
			char* immediateValueStr = WriteDecimalNumberToBuffer((int)immediateValue);
			immediateValueStr = WrapStringWithSymbols(immediateValueStr, '[', ']');

			byteIndex += isWide ? 3 : 2;

			fwrite(registerName, strlen(registerName), 1, decodedFile);
			fwrite(&commaSpace, strlen(commaSpace) * sizeof(char), 1, decodedFile);
			fwrite(immediateValueStr, strlen(immediateValueStr), 1, decodedFile);
		}
		else if (accumulatorToMemoryMovOpcode == (char)(firstByte >> 1))
		{
			const char movOpName[] = "mov ";
			fwrite(&movOpName, strlen(movOpName) * sizeof(char), 1, decodedFile);

			bool isWide = (firstByte & 0b00000001) == 0b00000001;

			const char* registerName = "ax";

			int immediateValue = isWide
				? *(short*)(instructionsBuffer + byteIndex + 1)
				: *(char*)(instructionsBuffer + byteIndex + 1);
			char* immediateValueStr = WriteDecimalNumberToBuffer((int)immediateValue);
			immediateValueStr = WrapStringWithSymbols(immediateValueStr, '[', ']');

			byteIndex += isWide ? 3 : 2;

			fwrite(immediateValueStr, strlen(immediateValueStr), 1, decodedFile);
			fwrite(&commaSpace, strlen(commaSpace) * sizeof(char), 1, decodedFile);
			fwrite(registerName, strlen(registerName), 1, decodedFile);
		}
		else
		{
			// unrecognized

			byteIndex += 2;
		}

		const char newLine[] = "\n";
		fwrite(&newLine, strlen(newLine), 1, decodedFile);
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