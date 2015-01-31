
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
#include "Test.h"

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
    printf("    --snapshot - Create a buildy.json containing revision information for \n");
    printf("                 reproducing a build.\n");
    printf("    --config <config_file_name> - Use provided path to buildy.json instead of\n");
    printf("                                  automatically finding it.\n");
    printf("    --superclean - Deletes devel_artifacts and all components build\n");
    printf("                   directories.\n");
    printf("    --test - Executes unit tests. Applies to provided tag, project name or ALL\n");
    printf("             if no arg provided.\n");
    printf("    --checkout <branch> - Executes git checkout <branch> on all components. If\n");
    printf("                          a component lacks a branch with that name, it will do\n");
    printf("                          nothing.\n");

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
        XString configFileName;

        XString overrideConfigPath;
        if( CheckOption( options, "--config", overrideConfigPath ) )
        {
            if( overrideConfigPath.Contains( PATH_SLASH ) )
            {
                size_t lastSlash = overrideConfigPath.rfind( PATH_SLASH );
                configDir = overrideConfigPath.substr( 0, lastSlash );
                configFileName = overrideConfigPath.substr( lastSlash + 1 );
            }
            else
            {
                configDir = ".";
                configFileName = overrideConfigPath;
            }
        }

        if( configDir.length() == 0 )
            X_THROW(("buildy.json not found."));

        XRef<Config> cfg = new Config( configDir, configFileName );

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
            GITStatus( cfg, arg, configDir );
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
        else if( CheckOption( options, "--superclean", arg ) )
        {
            SuperClean( cfg, arg );
        }
        else if( CheckOption( options, "--test", arg ) )
        {
            Test( cfg, arg );
        }
        else if( CheckOption( options, "--checkout", arg ) )
        {
            GITCheckout( cfg, "", configDir, arg );
        }
        else PrintHelp();
    }
    catch( std::exception& ex )
    {
        printf("%s\n",ex.what());
        fflush(stdout);
        exit(1);
    }

    return 0;
}
