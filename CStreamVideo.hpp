


template <typename T>
class ThreadSafeQueue {
public:
    void push(T value) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(value));
        cond_var_.notify_one();
    }

    bool pop(T& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_var_.wait(lock, [this] { return !queue_.empty(); });
        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }
    bool empty() {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
};

/*Class responsible for capturing video frames after initializing the webcam and enocding the frame data
to appropriate format */
class videoStream
{
public:
videoStream();
/*Open and Initilaize the camera using ffmpeg APIs*/
bool initializeCamera();
/*After camera initialization, allocate memory for frame and read framedata.
Store frame data in a char buffer and return char buffer containing single frame data*/
unsigned char* getFrameData(int& width, int& height);
/*Control the exit or end of life of the program by pressing,
a key from the keyboard*/
void listenForKeyPress();
/*cleanup camera resources allocated for reading frames from the camera*/
void cleanupCamera();
/*remux the final encoded video to ffmp4 format*/
bool remuxVideo(const char* inputFilename, const char* outputFilename); 
~videoStream();

AVFormatContext* m_formatContext;
AVCodecContext* m_decoderContext;
int m_videoStreamIndex;
};
/*This function handles video capture from a camera and encodes the captured frames into a video file using FFmpeg.
 It runs multiple threads to manage different tasks concurrently.*/
int  videoCaptureAndEncoding();
/*This is the entry point for a Windows application. Here’s what it does:

Register the Window Class: Defines the window class with a name and the WindowProc function as the window procedure.
Create the Window: Creates a window with the specified class name, title, and dimensions.
Create Buttons: Creates two buttons within the window:
Start/Stop Button: Initially labeled “Start”, used to start and stop video recording.
Play Button: Labeled “Play”, used to play the recorded video.
Show the Window: Makes the window visible.
Message Loop: Runs a loop that retrieves and dispatches messages to the window procedure until a WM_QUIT message is received.*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
/*function that processes messages sent to a window. 
WM_DESTROY: When the window is being destroyed, this message is sent. The function calls PostQuitMessage(0) to signal the end of the application and returns 0.
WM_COMMAND: This message is sent when a command is issued, such as a button click. The function checks the wParam to determine which button was clicked:
Start/Stop Button (ID 1): Toggles the recording state. If recording is active, it stops the recording and joins the video thread, then changes the button text to “Start”. If recording is inactive, it starts the recording in a new thread and changes the button text to “Stop”.
Play Button (ID 2): Opens the output.mp4 file using the default media player.
If the message is not handled by these cases, the function calls DefWindowProcW to handle the default processing.*/
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) ;

/*The RenderFrame function is responsible for rendering a video frame onto a specified window.*/
void RenderFrame(HWND hWnd, unsigned char* data, int width, int height);