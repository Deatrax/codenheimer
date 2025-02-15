#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>


/*
    This is a critial supportive file that handles accessing the appdata folders and other stuff.
    DO NOT TRY TO CHANGE ANYTHING IN HERE WITHOUT TELLING ME
*/

//DEFINES and DEAFULTS
// #define std:string string
using std::string;

namespace assist{

    #ifdef _WIN32   //===========WINDOWS FUNCTIONS================

    /**
     * @brief Get the AppData folder path for the app
     *
     * @param str the c-string that will be populated with the derived path
     */
    static void getAppData_folder(char* str){

        sprintf(str,"%s\\AppData\\Roaming\\KCATDVWSPJD", getenv("USERPROFILE"));
    }

    /**
     * @brief function to make any folder name into a folder path in the appdata folder
     *
     * @param filename the c-string that will be populated with the derived file path
     */
    static void make_appData_filePath(char*  filename){
        char str[257];
        sprintf(str,"%s\\AppData\\Roaming\\KCATDVWSPJD\\%s\0",getenv("USERPROFILE"),filename);
        strcpy(filename,str);
    }


    #elif defined(__APPLE__)  //===========UNIX/MacOS FUNCTIONS================

    /**
     * @brief Get the AppData folder path for the app
     *
     * @param str
     */
    static void getAppData_folder(char* str) {
        sprintf(str, "%s/Library/Application Support/KCATDVWSPJD", getenv("HOME"));
    }

    /**
     * @brief function to make any folder name into a folder path in the appdata folder
     *
     * @param filename the c-string that will be populated with the derived file path
     */
    static void make_appData_filePath(char* filename) {
        char str[400];
        sprintf(str, "%s/Library/Application Support/KCATDVWSPJD/%s", getenv("HOME"), filename);
        strcpy(filename, str);
    }

    #endif




    //includes for the ensure function
    #include <stdio.h>
    #include <errno.h>
    #ifdef _WIN32
    #include <direct.h>   // For _mkdir() on Windows
    #define mkdir _mkdir  // Use _mkdir on Windows
    #else
    #include <sys/stat.h>  // For mkdir() on macOS/Linux
    #endif

/**
 * @brief Creates a target directory and/or file, and verifies their existence.
 *
 * This function creates the specified directory and file, if they do not exist,
 * and verifies their existence. Directory creation is skipped if `dir_path` is null,
 * and file creation is skipped if `file_path` is null.
 *
 * @param dir_path The target directory path. Pass null to skip directory creation.
 * @param file_path The target file path. Pass null to skip file creation.
 * @param mode The file open mode, determining destructive or non-destructive opening.
 * @return int Returns 0 if successful, or -1 if failed to create either one of the targets.
 */
static int ensure_directory_and_open_file(const char *dir_path, const char *file_path, const char *mode) {
            //skip if null passed BECOZ this means that directory making is to be skipped
            if (dir_path == NULL || strlen(dir_path) == 0) {
                goto fileCheck;
            }

            // Try to create the directory
    #ifdef _WIN32
            if (mkdir(dir_path) == -1) {
    #else
            if (mkdir(dir_path, 0777) == -1) {
    #endif
                if (errno != EEXIST) {
                    perror("mkdir failed");
                    return -1;  // Directory creation failed and it doesn't already exist
                }
                // If errno is EEXIST, we know the directory already exists
            }

        fileCheck: //
            //check if null was passed for the filepath and it is to be skipped
            if (file_path == NULL || strlen(file_path) == 0) {
                return 0;
            }
            // Now that the directory exists, open the file
            FILE *file = fopen(file_path, mode);
            if (file == NULL) {
                perror("fopen failed");
                return -1;  // File opening failed
            }

            // Use the file...
            // Don't forget to close the file when done
            fclose(file);


            return 0;  // Success
    }

        // ========================== NEW FILE I/O FUNCTIONS ==========================

        /**
 * @brief Edits a specific line in a file.
 * Ensures the modified file does not contain unexpected newlines.
 *
 * @param filename The file to modify.
 * @param lineNumber The line number to modify (1-based index).
 * @param newLine The new content for the specified line.
 * @return true if successful, false if the operation fails.
 */
        static bool editLine(const std::string& filename, int lineNumber, const std::string& newLine) {
            std::ifstream fileIn(filename);
            if (!fileIn) return false;

            std::vector<std::string> lines;
            std::string line;
            int currentLine = 1;

            while (std::getline(fileIn, line)) {
                if (currentLine == lineNumber) {
                    lines.push_back(newLine);  // Replace the target line
                } else {
                    lines.push_back(line);
                }
                currentLine++;
            }
            fileIn.close();

            if (lineNumber > currentLine) return false;  // Line number out of range

            std::ofstream fileOut(filename, std::ios::trunc);
            if (!fileOut) return false;

            for (size_t i = 0; i < lines.size(); i++) {
                fileOut << lines[i];
                if (i != lines.size() - 1) fileOut << '\n';  // Prevent unexpected newlines
            }
            fileOut.close();
            return true;
        }

        /**
 * @brief Adds a new line at a specific position or appends at the end.
 *
 * @param filename The file to modify.
 * @param lineNumber The position to insert (-1 to append).
 * @param newLine The content to insert.
 * @return true if successful, false if operation fails.
 */
        static bool addLine(const std::string& filename, int lineNumber, const std::string& newLine) {
            std::ifstream fileIn(filename);
            if (!fileIn) return false;

            std::vector<std::string> lines;
            std::string line;
            int currentLine = 1;

            while (std::getline(fileIn, line)) {
                if (currentLine == lineNumber) {
                    lines.push_back(newLine); // Insert at the right position
                }
                lines.push_back(line);
                currentLine++;
            }
            fileIn.close();

            if (lineNumber == -1) {
                lines.push_back(newLine); // Append at the end
            }

            std::ofstream fileOut(filename, std::ios::trunc);
            if (!fileOut) return false;

            for (size_t i = 0; i < lines.size(); i++) {
                fileOut << lines[i];
                if (i != lines.size() - 1) fileOut << '\n';  // Prevent unexpected newlines
            }
            fileOut.close();
            return true;
        }

        /**
 * @brief Removes a specific line from a file.
 * Ensures that there are no unexpected newlines after deletion.
 *
 * @param filename The file to modify.
 * @param lineNumber The line number to remove.
 * @return true if successful, false if operation fails.
 */
        static bool removeLine(const std::string& filename, int lineNumber) {
            std::ifstream fileIn(filename);
            if (!fileIn) return false;

            std::vector<std::string> lines;
            std::string line;
            int currentLine = 1;

            while (std::getline(fileIn, line)) {
                if (currentLine != lineNumber) {
                    lines.push_back(line);
                }
                currentLine++;
            }
            fileIn.close();

            if (lineNumber > currentLine) return false;

            std::ofstream fileOut(filename, std::ios::trunc);
            if (!fileOut) return false;

            for (size_t i = 0; i < lines.size(); i++) {
                fileOut << lines[i];
                if (i != lines.size() - 1) fileOut << '\n';  // Prevent unexpected newlines
            }
            fileOut.close();
            return true;
        }

}


#endif // HELPERFUNCTIONS_H
