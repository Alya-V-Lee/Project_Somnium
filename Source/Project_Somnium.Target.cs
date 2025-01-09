

using UnrealBuildTool;
using System.Collections.Generic;

public class Project_SomniumTarget : TargetRules
{
	public Project_SomniumTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Project_Somnium" } );
	}
}
