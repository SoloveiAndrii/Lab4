#include <iostream>
#include <windows.h>
#include <filesystem>
#include <regex>
#include <cstdint>

using namespace std;

DWORD getFileAttributes(const filesystem::path& p)
{
	return GetFileAttributes(p.c_str());
}

int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	if (argc < 3 || argc % 2 != 1)
	{
		cout << "Ця програма використовується для підрахунку сумарного обсягу файлів у каталозі. Програма може:" << "\n";
		cout << "1.Працювати з вказаним каталогом (передається як параметр утиліти)." << "\n";
		cout << "2.Працювати як з усіма файлами каталогу, так і з вказаними файлами за шаблоном." << "\n";
		cout << "3.Інтерпретувати змінну кількість параметрів командного рядка." << "\n";
		cout << "4.Формувати коди завершення." << "\n";
		cout << "5.Враховувати атрибути файлу (прихований, лише для читання, архівний, без атрибуту)." << "\n";
		cout << "\nВикористання:\n";
		cout << "1.Кожна пара аргументів представляє директорію та відповідний шаблон." << "\n";
		cout << "2.У командному рядку програму слід викликати з непарною кількістю аргументів, що дорівнює або більше за 3." <<"\n";
		cout << "3.Структура у командному рядку: program /path/to/directory1 pattern1 /path/to/directory2 pattern2...\n";
		cout << "4.Приклад у командному рядку: lab4_1 \"D:\\\\Новая папка\" \".*\\.docx$\"\n";
		cout << "5.У Microsoft Visual Studio програму слід викликати з парною кількістю аргументів, що дорівнює або більше за 2." << "\n";
		cout << "6.Структура у Microsoft Visual Studio: /path/to/directory1 pattern1 /path/to/directory2 pattern2...\n";
		cout << "7.Приклад у Microsoft Visual Studio: \"D:\\\\Новая папка\" \".*\\.docx$\"\n";
		return 1;
	}

	for (int i = 1; i < argc; i += 2)
	{
		string path = argv[i];
		string pattern = argv[i + 1];
		regex re(pattern);

		uintmax_t total_size = 0;
		uintmax_t hidden_size = 0;
		uintmax_t readonly_size = 0;
		uintmax_t archive_size = 0;
		uintmax_t noattr_size = 0;
		uintmax_t pattern_size = 0;

		for (const auto& entry : filesystem::recursive_directory_iterator(path))
		{
			if (entry.is_regular_file())
			{
				DWORD attributes = getFileAttributes(entry.path());
				if (attributes == INVALID_FILE_ATTRIBUTES)
				{
					noattr_size += entry.file_size();
				}
				else
				{
					if (attributes & FILE_ATTRIBUTE_HIDDEN)
					{
						hidden_size += entry.file_size();
					}
					if (attributes & FILE_ATTRIBUTE_READONLY)
					{
						readonly_size += entry.file_size();
					}
					if (attributes & FILE_ATTRIBUTE_ARCHIVE)
					{
						archive_size += entry.file_size();
					}
				}
				if (regex_match(entry.path().filename().string(), re))
				{
					pattern_size += entry.file_size();
				}
				total_size += entry.file_size();
			}
		}

		cout << "Директорія: " << path << ", Шаблон: " << pattern << "\n";
		cout << "Загальний розмір файлів у директорії: " << total_size / 1024 << " KB" << "\n";
		cout << "Загальний розмір файлів, що відповідають шаблону '" << pattern << "': " << pattern_size / 1024 << " KB" << "\n";
		cout << "Загальний розмір прихованих файлів: " << hidden_size / 1024 << " KB" << "\n";
		cout << "Загальний розмір файлів лише для читання: " << readonly_size / 1024 << " KB" << "\n";
		cout << "Загальний розмір архівних файлів: " << archive_size / 1024 << " KB" << "\n";
		cout << "Загальний розмір файлів без атрибутів: " << noattr_size / 1024 << " KB" << "\n\n";
	}
}