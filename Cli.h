/*
 * Command-Line Interpreter
 *
 * This source file can be found under:
 * http://www.github.com/arduino-library/Cli
 *
 * Please visit:
 *   http://www.microfarad.de
 *   http://www.github.com/microfarad-de
 *   http://www.github.com/arduino-library
 *
 * Copyright (C) 2025 Karim Hraibi (khraibi at gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CLI_H_
#define __CLI_H_

#include <Arduino.h>

#define CLI_NUM_CMD         15   // Number of cli commands
#define CLI_NUM_ARG          3   // Number of cli arguments
#define CLI_ARG_LEN          8   // Maximum argument length
#define CLI_PRINTF_BUF_SIZE 40   // Size of the printf buffer



/*
 * CLI command structure
 */
typedef struct
{
  const char *str;          // Command name
  const char *doc;          // Command description
  int (*fct)(int, char **); // Pointer to command function
} CliCmd_s;


/*
 * Command line interpreter class
 */
class CliClass
{
  public:
    /*
     * Initialize a CLI object
     * Note: this function calls Serial.begin
     */
    void init (
      uint32_t serialBaud,    // Serial Baud rate
      bool     echo = false,  // Enable echo
      void(*helpCallback)(void) = nullptr  // Callback returns additional help text
      );

    /*
     * Define a new CLI command
     */
    int newCmd (
      const char *name,             // Command name
      const char *description,      // Command description
      int (*function)(int, char **) // Pointer to a function to be executed by this command
      );

    /*
     * Prompt user for a command - non-blocking function
     * Return: EXIT_FAILURE or the return value of the command function
     *
     * If user enters "h" or "help" a help screen with a list of commands and
     * their descriptions will be displayed.
     *
     * If two commands point to the same function, only the first one will be
     * displayed in the help screen.
     */
    int getCmd (void);

    /*
     * Print a list of available commands and their description
     */
    void showHelp (void);

    /*
     * Sort commands in alphabetical order
     */
    void sortCmds (void);

    /*
     * Emulate stdio functions
     */
    void xprintf (const char *fmt, ... );
    void xputs (const char *c);
    void xputchar (int c);
    int xgetchar (void);


  private:
    void textPadding (char c, int size);                              // Insert repeated sequence of characters
    void textPrintBlock (const char *text, int lineSize, int offset); // Print a formatted block of text
    CliCmd_s cmd[CLI_NUM_CMD];             // Array of commands
    void(*helpCallback)(void);             // Callback returns additional help text
    char *argv[CLI_NUM_ARG];               // Array of pointers to argument strings
    int argc = 0;                          // Number of arguments
    int idx = 0;                           // Character index
    enum {START, DISCARD_LEADING_SPACES, CAPTURE_STRING, DISCARD_TRAILING_SPACES, EVALUATE} state = START; // State
    bool initialized = false;              // Initialization status
    bool echo = false;                     // Enable echo
    char argBuf[CLI_NUM_ARG][CLI_ARG_LEN]; // Array of charactars for commands and arguments
    char printfBuf[CLI_PRINTF_BUF_SIZE];   // Temporary buffer used by xprintf();
    unsigned char numCmds;                 // Total number of commands
    unsigned char indent = 0;              // For indentation
};


/*
 * CLI object as as singleton
 */
extern CliClass Cli;


#endif /* __CLI_H_ */
