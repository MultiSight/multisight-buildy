
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
    }
    catch( std::exception& ex )
    {
        printf("%s\n",ex.what());
        fflush(stdout);
        exit(0);
    }

    return 0;
}
