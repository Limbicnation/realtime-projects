using UnrealBuildTool;

public class empire_of_dirt : ModuleRules
{
    public empire_of_dirt(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
    
        PublicDependencyModuleNames.AddRange(new string[] { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore",
            "UMG",
            "Niagara"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { 
            "Slate",
            "SlateCore"
        });

        PublicIncludePaths.AddRange(new string[] {
            ModuleDirectory
        });

        IWYUSupport = IWYUSupport.Full;
        bLegacyPublicIncludePaths = false;
    }
}
