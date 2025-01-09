

using UnrealBuildTool;
using System.Collections.Generic;

public class Project_SomniumEditorTarget : TargetRules
{
	public Project_SomniumEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Project_Somnium" } );
	}
}
