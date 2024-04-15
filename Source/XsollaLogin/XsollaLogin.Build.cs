// Copyright 2023 Xsolla Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class XsollaLogin : ModuleRules
{
    public XsollaLogin(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "HTTP",
                "Json",
                "JsonUtilities",
                "UMG",
                "OnlineSubsystem",
                "XsollaWebBrowser",
                "XsollaUtils",
                "XsollaSettings"
            }
            );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
            );

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/Android"));

            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });
            #if false // WITH_DIRECTIVE (Potential workaround for duplicate class error in Android builds on TeamCity)
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "XsollaLogin_UPL.xml"));
            #endif
        }
        if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);

            AdditionalPropertiesForReceipt.Add("IOSPlugin", Path.Combine(PluginPath, "XsollaLogin_IOS_UPL.xml"));

            PublicAdditionalFrameworks.Add(
            new Framework(
                "XsollaSDKLoginKit",
                "../IOS/XsollaSDKLoginKit.embeddedframework.zip",
                "XsollaSDKLoginKit.framework" // WITH_DIRECTIVE
            )
            );
            PublicAdditionalFrameworks.Add(
            new Framework(
                "XsollaSDKLoginKitObjectiveC",
                "../IOS/XsollaSDKLoginKitObjectiveC.embeddedframework.zip",
                "XsollaSDKLoginKitObjectiveC.framework" // WITH_DIRECTIVE
            )
            );
            PublicAdditionalFrameworks.Add(
            new Framework(
                "XsollaSDKUtilities",
                "../IOS/XsollaSDKUtilities.embeddedframework.zip",
                "XsollaSDKUtilities.framework" // WITH_DIRECTIVE
            )
            );
        }

        PublicDefinitions.Add("WITH_XSOLLA_LOGIN=1");
    }
}
