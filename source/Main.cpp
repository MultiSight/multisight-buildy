
#include <stdio.h>
#include <stdlib.h>
#include <list>

#include "XSDK/XString.h"
#include "XSDK/XPath.h"
#include "XSDK/OS.h"

#include "Config.h"
#include "Option.h"
#include "GIT.h"
#include "Build.h"

using namespace XSDK;
using namespace std;

void PrintHelp()
{
    printf("usage: buildy <command> <arg>\n\n");
    printf("commands -\n");
    printf("    --clone - Execute git clone. Applies to provided tag, project name or ALL\n");
    printf("              if no arg provided.\n");
    printf("    --pull - Execute git pull. Applies to provided tag, project name or ALL\n");
    printf("             if no arg provided.\n");
    printf("    --status - Execute git status. Applies to provided tag, project name or\n");
    printf("               ALL if no arg provided.\n");
    printf("    --build - cleanbuild script on each project matching tag, project name or\n");
    printf("              ALL if no arg provided.\n");
    exit(0);
}

int main( int argc, char* argv[] )
{
    try
    {
        list<struct Option> options = ParseOptions( argc, argv );

        XString configPath = "buildy.json";
        CheckOption( options, "--config", configPath ); // could change configPath

        XRef<Config> cfg = new Config( configPath );

        XString arg;
        if( CheckOption( options, "--clone", arg ) )
        {
            GITClone( cfg, arg );
        }
        else if( CheckOption( options, "--pull", arg ) )
        {
            GITPull( cfg, arg );
        }
        else if( CheckOption( options, "--status", arg ) )
        {
            GITStatus( cfg, arg );
        }
        else if( CheckOption( options, "--build", arg ) )
        {
            XString arg2;

            CleanBuild( cfg,
                        arg,
                        CheckOption( options, "--release", arg2 ),
                        CheckOption( options, "--superclean", arg2 ) );
        }
        else if( CheckOption( options, "--snapshot", arg ) )
        {
            if( arg.length() == 0 )
                X_THROW(("--snapshot requested without providing an output path."));

            BuildySnapshot( cfg, arg );
        }
        else PrintHelp();
    }
    catch( std::exception& ex )
    {
        printf("%s\n",ex.what());
        fflush(stdout);
        exit(0);
    }

    return 0;
}
