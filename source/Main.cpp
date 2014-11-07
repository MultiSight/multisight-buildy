
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

XString FindConfigDir()
{
    if( XPath::Exists( "buildy.json" ) )
        return XString::Format( ".%s", PATH_SLASH );

    XPath path( "." );

    for( path.IterStart(); path.IterValid(); path.Iterate() )
    {
        XString entryName = *path.IterData();

        if( XPath::IsDir( entryName ) )
        {
            XString potentialPath = XString::Format( "%s%s", entryName.c_str(), PATH_SLASH );
            if( XPath::Exists( potentialPath + "buildy.json" ) )
                return potentialPath;
        }
    }

    return "";
}

int main( int argc, char* argv[] )
{
    try
    {
        list<struct Option> options = ParseOptions( argc, argv );

        XString configDir = FindConfigDir();

        XString overrideConfigPath;
        if( CheckOption( options, "--config", overrideConfigPath ) )
        {
            size_t lastSlash = overrideConfigPath.rfind( PATH_SLASH );

            if( lastSlash != string::npos )
                configDir = overrideConfigPath.substr( 0, lastSlash );
        }

        if( configDir.length() == 0 )
            X_THROW(("buildy.json not found."));

        printf("configDir = %s\n",configDir.c_str());
        fflush(stdout);

        XRef<Config> cfg = new Config( configDir );

        XString arg;
        if( CheckOption( options, "--clone", arg ) )
        {
            bool excludeBranch = CheckOption( options, "--exclude-branch" );
            bool excludeRev = CheckOption( options, "--exclude-rev" );

            GITClone( cfg, arg, excludeBranch, excludeRev );
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
