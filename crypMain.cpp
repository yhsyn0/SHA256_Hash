// <>
#include "hw2_1_sha256.h"

#ifndef WINDOWS
    #include <windows.h>

#else
    #include <termios.h>
    #include <unistd.h>

#endif

int main()
{
    ifstream inFile;
    ofstream outFile;
    inFile.open("db.txt");
    outFile.open("db.txt", ios::app);
	fstream file("db.txt", std::ios::in | std::ios::out);

    int selection = 0;
    cout << "Please choose your transaction with numbers 1-2 : \n" << "1. Signup \n2. Login" << endl;
    cin >> selection;

    if(selection == 1)
    {
        string userName;
        string pass;

        UserName:
            cout << "Please enter a username : " << endl;
            cin >> userName;
            if(userName.length() == 0)
            {
                cout << "User name can not be empty !" << endl;
                goto UserName;
            }
			else 
			{
                ifstream inFileTemp;
                inFileTemp.open("db.txt");
				string line;
				while(getline(inFileTemp, line))
				{
					if(line.find("Username : ") != string::npos)
					{
						if(line.substr(line.find("Username : ") + 11) == userName)
                        {
                            cout << "Username has already taken, please enter another username !" << endl;
                            goto UserName;
                        }
					}

					if(inFileTemp.eof())
                    {
                        inFileTemp.close();
						break;
                    }
				}
			}
        Password:
            cout << "Please enter a password : " << endl;
            cin >> pass;
            if(pass.length() == 0)
            {
                cout << "Password can not be empty !" << endl;
                goto Password;
            }

        int userCounter;
        if(inFile.peek() == EOF)
        {
            userCounter = 0;
            outFile << "0" << endl << endl;
        }
        else
        {
            string line;
            getline(inFile, line);
            userCounter = line[0] - '0' + 1;
			file.seekp(0);
			file << to_string(userCounter);
        }

        outFile << "User " << to_string(userCounter) << ":" << endl;
        outFile << "\tUsername : " << userName << endl;
        outFile << "\tPassword : " << SHA256(&pass[0]) << endl << endl;
    }

	else if(selection == 2)
	{
        string userName;
        string pass;

        cout << "Please enter your username and password :" << endl;
        cout << "Username : ";
        cin >> userName;

        cout << "Password : ";

        #ifndef WINDOWS
            HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
            DWORD mode = 0;
            GetConsoleMode(hStdin, &mode);

            SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
            cin >> pass;
            SetConsoleMode(hStdin, mode);

        #else
            termios oldt;
            tcgetattr(STDIN_FILENO, &oldt);
            termios newt = oldt;
            newt.c_lflag &= ~ECHO;

            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
            cin >> pass;
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        #endif

        string line;
        while(getline(inFile, line))
        {
            if(line.find("Username : ") != string::npos)
            {
                if(line.substr(line.find("Username : ") + 11) == userName)
                {
                    string passHash;
                    getline(inFile, passHash);
                    if(passHash.substr(line.find("Password : ") + 11).compare(SHA256(&pass[0])))
                    {
                        cout << "\nLogin Successful" << endl;

                        inFile.close();
                        outFile.close();
                        file.close();

                        return 1;
                    }
                    else
                    {
                        cout << "\nLogin Failed" << endl;

                        inFile.close();
                        outFile.close();
                        file.close();

                        return -1;
                    }
                }
            }
        }
	}
    
    if(selection == 2)
        cout << "\nLogin Failed" << endl;

    return 0;
}