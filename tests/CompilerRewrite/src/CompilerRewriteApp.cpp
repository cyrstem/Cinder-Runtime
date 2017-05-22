#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "runtime/CompilerMSVC.h"
#include "Watchdog.h"
#include "Test.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CompilerRewriteApp : public App {
public:
	void setup() override;
	void draw() override;
	
	void buildTestCpp();

	unique_ptr<Test> mTest;
	Font mFontLarge, mFontSmall;
	rt::CompilerRef mCompiler;
};

void CompilerRewriteApp::setup()
{
	mFontSmall = Font( "Arial", 15 );
	mFontLarge = Font( "Arial", 35 );
	mCompiler = rt::Compiler::create();
	mTest = make_unique<Test>();
	
	wd::watch( "C:\\CODE\\CINDER\\CINDER_FORK\\BLOCKS\\CINDER-RUNTIME\\TESTS\\COMPILERREWRITE\\SRC\\TEST.CPP", [this]( const fs::path &path ) { buildTestCpp(); } );
}

void CompilerRewriteApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
	
	gl::drawStringCentered( mTest->getString(), getWindowCenter(), ColorA::white(), mFontLarge );
	gl::drawStringCentered( PROJECT_PATH, getWindowCenter() + vec2( 0,30 ), ColorA::white(), mFontSmall );
}

void CompilerRewriteApp::buildTestCpp()
{
	fs::path outputDirectory = PROJECT_DIR / fs::path( "build" ) / PLATFORM / CONFIGURATION;
	fs::path intermediateDirectory = outputDirectory / fs::path( "intermediate" );

	// compiler args
	std::string command = "cl ";
	command += " /Fd\"" + intermediateDirectory.string() + "\\VC140_RT.PDB\" ";
	command += " /Fo\"" + intermediateDirectory.string() + "\\\\\""; 
	command += "/Zi /nologo /W3 /WX- /Od /D CINDER_SHARED /D WIN32 /D _WIN32_WINNT=0x0601 /D _WINDOWS /D NOMINMAX /D _DEBUG /D _UNICODE /D UNICODE /Gm /EHsc /RTC1 /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Zc:inline /Gd /TP ";
	command += "/I..\\INCLUDE /I..\\..\\..\\..\\..\\INCLUDE /I..\\..\\..\\INCLUDE /I..\\BLOCKS\\WATCHDOG\\INCLUDE ";
	command += " C:\\CODE\\CINDER\\CINDER_FORK\\BLOCKS\\CINDER-RUNTIME\\TESTS\\COMPILERREWRITE\\SRC\\TEST.CPP";
	
	// linker args
	command += " /link ";
	command += " /OUT:" + intermediateDirectory.string() + "\\test.dll";
	command += " /IMPLIB:" + intermediateDirectory.string() + "\\test.lib";
	command += " /INCREMENTAL:NO /NOLOGO";
	command += " /LIBPATH:..\\..\\..\\..\\..\\LIB\\MSW\\" + string( PLATFORM );
	command += " /LIBPATH:..\\..\\..\\..\\..\\LIB\\MSW\\" + string( PLATFORM ) + "\\" + string( CONFIGURATION ) + "\\V140\\";
	command += " CINDER.LIB OPENGL32.LIB KERNEL32.LIB USER32.LIB GDI32.LIB WINSPOOL.LIB COMDLG32.LIB ADVAPI32.LIB SHELL32.LIB OLE32.LIB OLEAUT32.LIB UUID.LIB ODBC32.LIB ODBCCP32.LIB /NODEFAULTLIB:LIBCMT /NODEFAULTLIB:LIBCPMT ";
	command += " /MACHINE:X64"; // PLATFORM ?;
#if defined( _DEBUG )
	command += " /PDB:\"" + intermediateDirectory.string() + "\\VC140_RT.PDB\"";
	command += " /DEBUG";
#endif
	command += " /DLL ";

	mCompiler->build( command, []( const rt::CompilationResult &result ) {
		app::console() << "Done!" << endl;
	} );
}

CINDER_APP( CompilerRewriteApp, RendererGl() )
