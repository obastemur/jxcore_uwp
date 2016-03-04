#include "pch.h"
#include "MainPage.xaml.h"
#include "jxcore/jx.h"

#include <stdio.h>  /* defines FILENAME_MAX */
#include <direct.h>
#define GetCurrentDir _getcwd

using namespace App1;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

static inline void DebuggerOutput_(const char* ctstr, ...) {
  char str[8192];
  va_list ap;
  va_start(ap, ctstr);
  int pos = vsnprintf(str, 8192, ctstr, ap);
  va_end(ap);
  str[pos] = '\0';

  OutputDebugStringA(str);
}
#define log_console(...) DebuggerOutput_(__VA_ARGS__)


void callback(JXResult *results, int argc) { }

App::App()
{
  InitializeComponent();
  Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);
}

void App::OnTick(Platform::Object^ sender, Platform::Object^ e)
{
  JX_LoopOnce();
}

void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e)
{
  auto rootFrame = dynamic_cast<Frame^>(Window::Current->Content);

  // Do not repeat app initialization when the Window already has content,
  // just ensure that the window is active
  if (rootFrame == nullptr)
  {
    // Create a Frame to act as the navigation context and associate it with
    // a SuspensionManager key
    rootFrame = ref new Frame();

    rootFrame->NavigationFailed += ref new Windows::UI::Xaml::Navigation::NavigationFailedEventHandler(this, &App::OnNavigationFailed);

    if (e->PreviousExecutionState == ApplicationExecutionState::Terminated)
    {
      // TODO: Restore the saved session state only when appropriate, scheduling the
      // final launch steps after the restore is complete

    }

    if (rootFrame->Content == nullptr)
    {
      // When the navigation stack isn't restored navigate to the first page,
      // configuring the new page by passing required information as a navigation
      // parameter
      rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
    }
    // Place the frame in the current Window
    Window::Current->Content = rootFrame;
    // Ensure the current window is active
    Window::Current->Activate();
  }
  else
  {
    if (rootFrame->Content == nullptr)
    {
      // When the navigation stack isn't restored navigate to the first page,
      // configuring the new page by passing required information as a navigation
      // parameter
      rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
    }
    // Ensure the current window is active
    Window::Current->Activate();
  }

  char cCurrentPath[FILENAME_MAX];

  if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
  {
    memcpy(cCurrentPath, "/jx", 3);
    cCurrentPath[3] = '\0';
  }

  JX_Initialize(cCurrentPath, callback);
  //// Creates a new engine for the current thread
  //// It's our first engine instance hence it will be the
  //// parent engine for all the other engine instances.
  //// If you need to destroy this engine instance, you should
  //// destroy everything else first. For the sake of this sample
  //// we have our first instance sitting on the main thread
  //// and it will be destroyed when the app exists.
  JX_InitializeNewEngine();

  char *contents = "global.window = {setTimeout:global.setTimeout};\n"
    "var fs = require('fs');var path = require('path');\n"
    "var assets = path.join(process.cwd(), 'Assets');\n"
    "var dirs = fs.readdirSync(assets);\n"
    "console.log('>>>>>>', process.cwd(), dirs);\n"
    "var main_file = path.join(assets, 'app.js');"
    "require(main_file);";

  //// define the entry file contents
  JX_DefineMainFile(contents);

  JX_StartEngine();

  JX_LoopOnce();

  DispatcherTimer^ timer = ref new DispatcherTimer;
  timer->Tick += ref new Windows::Foundation::EventHandler<Platform::Object ^>(this, &App1::App::OnTick);
  TimeSpan t;
  t.Duration = 10;
  timer->Interval = t;
  timer->Start();
}

/// <summary>
/// Invoked when application execution is being suspended.	Application state is saved
/// without knowing whether the application will be terminated or resumed with the contents
/// of memory still intact.
/// </summary>
/// <param name="sender">The source of the suspend request.</param>
/// <param name="e">Details about the suspend request.</param>
void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{
  (void)sender;	// Unused parameter
  (void)e;	// Unused parameter

            //TODO: Save application state and stop any background activity
}

/// <summary>
/// Invoked when Navigation to a certain page fails
/// </summary>
/// <param name="sender">The Frame which failed navigation</param>
/// <param name="e">Details about the navigation failure</param>
void App::OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e)
{
  throw ref new FailureException("Failed to load Page " + e->SourcePageType.Name);
}