// STD::ASYNC TUTORIAL
//   Example 1 of ordinar function call vs. std::async-separate-thread-call (see ZZ1)
//   Example 2 of std::async-separate-thread-call: function vs. lambda (see ZZ2)
//   Example 3 of fetch data from file: ordinar function call vs. std::async-possible-thread-function-object-call (see ZZ3)

// thispointer.com/c11-multithreading-part-9-stdasync-tutorial-example/

#define FETCH_DB_IN_SEPARATE_TREAD 1 // 1 - program will take 5 seconds to complete,
                                     // 0 - program will take 10 seconds to complete

#if FETCH_DB_IN_SEPARATE_TREAD
    #define SEPARATE_THREAD_USE_LAMBDA 1
#endif

#define FETCH_FILE_USE_FUNCTOR 1

#include <chrono>
#include <iostream>

#if FETCH_DB_IN_SEPARATE_TREAD || FETCH_FILE_USE_FUNCTOR
    #include <future>
#endif


#include <string>
#include <thread>

using namespace std::chrono;

std::string fetchDataFromDB(std::string recvdData)
{
    // Make sure that function takes 5 seconds to complete
    std::this_thread::sleep_for(seconds(5));

    //Do stuff like creating DB Connection and fetching Data
    return "DB_" + recvdData;
}

std::string fetchDataFromFile(std::string recvdData)
{
    // Make sure that function takes 5 seconds to complete
    std::this_thread::sleep_for(seconds(5));

    //Do stuff like fetching Data File
    return "File_" + recvdData;
}

/*
 * Function Object
 */
struct DataFetcher
{
    std::string operator()(std::string recvdData) {
        // Make sure that function takes 5 seconds to complete
        std::this_thread::sleep_for(seconds(5));

        // Do stuff like fetching Data File
        return "File_" + recvdData;
    }
};

int main()
{
    // Get Start Time
    system_clock::time_point start = system_clock::now();

#if !FETCH_DB_IN_SEPARATE_TREAD // ZZ1
    // Fetch Data from DB
    std::string dbData = fetchDataFromDB("Data"); // 5 seconds to complete, then dbData == "DB_Data" 

    // Fetch Data from File
    std::string fileData = fetchDataFromFile("Data"); // 5 seconds to complete, then fileData == "File_Data"
#else
    // Execute task asynchronously and go to the next line
    std::future<std::string> resultFromDB = std::async(
        std::launch::async,
#if SEPARATE_THREAD_USE_LAMBDA // ZZ2
        [] (std::string recvdData) {
            std::this_thread::sleep_for (seconds(5));
            // Do stuff like creating DB Connection and fetching Data
            return "DB_" + recvdData;
        },
#else
        fetchDataFromDB,
#endif
        "Data"
    ); // 0 seconds to complete

    // Fetch Data from File
#if !FETCH_FILE_USE_FUNCTOR // ZZ3
    std::string fileData = fetchDataFromFile("Data"); // 5 seconds to complete, then fileData == "File_Data"
#else
    // If we do not specify an launch policy its behaviour will be similar to std::launch::async | std::launch::deferred.
    // With this launch policy it can run asynchronously or not depending on the load on system. But we have no control over it
    //                                               ↓
    std::future<std::string> fileResult = std::async(DataFetcher(), "Data");
    std::string fileData = fileResult.get();
#endif

    // Fetch Data from DB
    std::string dbData = resultFromDB.get(); // Will block till data is available in future<std::string> object.
#endif

    // Get End Time
    auto end = system_clock::now();

    auto diff = duration_cast<std::chrono::seconds>(end - start).count();

    std::cout << "Total Time Taken = " << diff << " Seconds" << std::endl;

    // Combine The Data
    std::string data = dbData + " :: " + fileData;

    // Printing the combined Data
    std::cout << "Data = " << data << std::endl;

    return 0;
}