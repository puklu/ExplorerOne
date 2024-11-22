/**
 * @file Trace.hpp
 * @brief Provides a logging utility for debugging and tracing information through USART.
 *
 * The `Trace` class enables formatted output to a USART pin for debugging purposes. 
 * It supports logging with optional metadata (module names) and file/line details for tracing.
 *
 * Usage:
 * - Use `TRACE_LOG` for detailed trace logging with file and line information.
 * - Use `PRINT` for simple formatted printing.
 * - Use `TRACE` for logging with module-specific context.
 *
 * Example:
 * ```
 * TRACE_LOG("Error code: %d", errorCode);
 * TRACE("MODULE_NAME", "Initialization completed successfully");
 * PRINT("Hello, World!");
 * ```
 */

#pragma once

#include "drivers/usart/UsartPin.hpp"

/**
 * @brief Macro for logging with file and line information.
 * 
 * @param format Format string (like in `printf`).
 * @param ... Variable arguments for the format string.
 */
#define TRACE_LOG(format, ...) Trace::printWithDetails(__FILE__, __LINE__, format, ##__VA_ARGS__)

/**
 * @brief Macro for simple formatted output.
 * 
 * @param format Format string (like in `printf`).
 * @param ... Variable arguments for the format string.
 */
#define PRINT(format, ...) Trace::print(format, ##__VA_ARGS__)

/**
 * @brief Macro for logging with a module context.
 * 
 * @param module Name of the module or context.
 * @param format Format string (like in `printf`).
 * @param ... Variable arguments for the format string.
 */
#define TRACE(module, format, ...) Trace::printWithMetadata(module, format, ##__VA_ARGS__)


/**
 * @class Trace
 * @brief A class for logging messages via USART.
 *
 * The `Trace` class provides methods for logging with different levels of detail, 
 * including simple logs, logs with module names, and detailed logs with file and line information.
 */
class Trace
{
public:
    /**
     * @brief Initializes the `Trace` class.
     *
     * This sets up the USART pin for logging. Should be called once before any logging.
     */
    static void initialize();

    /**
     * @brief Logs a formatted message.
     * 
     * @param format Format string (like in `printf`).
     * @param ... Variable arguments for the format string.
     */
    static void print(const char* format, ...);

    /**
     * @brief Logs a message with a module context.
     * 
     * @param module Name of the module or context.
     * @param format Format string (like in `printf`).
     * @param ... Variable arguments for the format string.
     */
    static void printWithMetadata(const char* module, const char* format, ...);

        /**
     * @brief Logs a message with file and line details.
     * 
     * @param file The file name where the log is generated.
     * @param line The line number in the file where the log is generated.
     * @param format Format string (like in `printf`).
     * @param ... Variable arguments for the format string.
     */
    static void printWithDetails(const char* file, int line, const char* format, ...);

private:
    static UsartPin* printUsartPin; /*!< Pointer to the USART pin used for logging. */
    static bool isInitialized;      /*!< Flag to indicate if the Trace class is initialized. */

};
