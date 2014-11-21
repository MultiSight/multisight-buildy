
#include "Build.h"
#include "Utils.h"
#include "XSDK/XPath.h"

using namespace XSDK;
using namespace std;

void SuperClean( XRef<Config> cfg, const XSDK::XString& tag )
{
    int err = 0;

    printf("Removing devel_artifacts...");
    fflush(stdout);

    err = system( "rm -rf devel_artifacts" );
    if( err < 0 )
        X_THROW(("Unable to remove devel_artifacts."));

    XString configDir = cfg->GetConfigDir();

    list<struct Component> components=(tag.length()>0) ? cfg->GetMatchingComponents(tag) : cfg->GetAllComponents();

    list<struct Component>::iterator i;
    for( i = components.begin(); i != components.end(); i++ )
    {
        if( i->cleanbuild.length() > 0 )
        {
            printf("%s\n",i->cleanbuild.c_str());
            fflush(stdout);

            err = system( XString::Format( "\"%s%s%s\" %s %s %s %s",
                                           configDir.c_str(),
                                           PATH_SLASH,
                                           i->cleanbuild.c_str(),
                                           i->name.c_str(),
                                           (i->src.length() > 0) ? i->src.c_str() : "NO_SOURCE",
                                           i->path.c_str(),
                                           "CLEAN" ).c_str() );
            if( err != 0 )
                X_THROW(("Build command failure."));
        }
        else if( i->cleanbuildContents.length() > 0 )
        {
#ifdef IS_WINDOWS
            XString fileName = ".\\embedded_build.bat";
#else
            XString fileName = "./embedded_build.sh";
#endif
            if( XPath::Exists( fileName ) )
                remove( fileName.c_str() );

            WriteFileContents( fileName, i->cleanbuildContents );

#ifdef IS_LINUX
            err = system( XString::Format( "chmod +x %s",
                                           fileName.c_str() ).c_str() );
            if( err < 0 )
                X_THROW(("Build command failure."));
#endif

            err = system( XString::Format( "%s %s %s %s %s",
                                           fileName.c_str(),
                                           i->name.c_str(),
                                           (i->src.length() > 0) ? i->src.c_str() : "NO_SOURCE",
                                           i->path.c_str(),
                                           "CLEAN" ).c_str() );
            if( err != 0 )
                X_THROW(("Build command failure."));

            if( XPath::Exists( fileName ) )
                remove( fileName.c_str() );
        }
        else X_THROW(( "No build script file specified and no cleanbuild_contents specified." ));
    }
}

void CleanBuild( XRef<Config> cfg, const XSDK::XString& tag, bool release, bool removeDevelArtifacts )
{
    int err = 0;
    if( removeDevelArtifacts )
    {
        printf("Removing devel_artifacts...");
        fflush(stdout);

        err = system( "rm -rf devel_artifacts" );
	if( err < 0 )
	  X_THROW(("Unable to remove devel_artifacts."));
    }

    XString configDir = cfg->GetConfigDir();

    list<struct Component> components=(tag.length()>0) ? cfg->GetMatchingComponents(tag) : cfg->GetAllComponents();

    list<struct Component>::iterator i;
    for( i = components.begin(); i != components.end(); i++ )
    {
        if( i->cleanbuild.length() > 0 )
        {
            printf("%s\n",i->cleanbuild.c_str());
            fflush(stdout);

            err = system( XString::Format( "\"%s%s%s\" %s %s %s %s",
                                           configDir.c_str(),
                                           PATH_SLASH,
                                           i->cleanbuild.c_str(),
                                           i->name.c_str(),
                                           (i->src.length() > 0) ? i->src.c_str() : "NO_SOURCE",
                                           i->path.c_str(),
                                           (release)?"RELEASE":"DEBUG" ).c_str() );
            if( err != 0 )
                X_THROW(("Build command failure."));
        }
        else if( i->cleanbuildContents.length() > 0 )
        {
#ifdef IS_WINDOWS
            XString fileName = ".\\embedded_build.bat";
#else
            XString fileName = "./embedded_build.sh";
#endif
            if( XPath::Exists( fileName ) )
                remove( fileName.c_str() );

            WriteFileContents( fileName, i->cleanbuildContents );

#ifdef IS_LINUX
            err = system( XString::Format( "chmod +x %s",
                                           fileName.c_str() ).c_str() );
            if( err < 0 )
                X_THROW(("Build command failure."));
#endif

            err = system( XString::Format( "%s %s %s %s %s",
                                           fileName.c_str(),
                                           i->name.c_str(),
                                           (i->src.length() > 0) ? i->src.c_str() : "NO_SOURCE",
                                           i->path.c_str(),
                                           (release)?"RELEASE":"DEBUG" ).c_str() );
            if( err != 0 )
                X_THROW(("Build command failure."));

            if( XPath::Exists( fileName ) )
                remove( fileName.c_str() );
        }
        else X_THROW(( "No build script file specified and no cleanbuild_contents specified." ));
    }
}
