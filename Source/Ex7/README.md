<a name="HOLTop" />
# Using the ANGLE NuGet Package #

---


<a name="Exercise7" />
### Exercise 7: Using the ANGLE NuGet Package ###

The **recommended** way to use ANGLE is to add it to your Visual Studio project using NuGet. However, the ANGLE project currently does not provide a template that uses NuGet. However, it is relatively easy to modify the Visual Studio project to use the NuGet version of ANGLE.  Using the NuGet version of ANGLE has the following advantages:

1. The builds released to NuGet have been tested by the ANGLE team and are considered stable releases.
1. The NuGet builds are referenced builds that can be used by the ANGLE team for investigating issues you may report.
1. The NuGet packages are easier to add to your project and save you the time of having to build ANGLE from source.


In this exercise you will complete the following steps:

1. Modify the Breakout project to remove references to the prebuilt version of ANGLE.
1. Use Nuget to add ANGLE to your project.

#### Task 1 - Removing Prebuilt ANGLE references from your project ####

The default ANGLE Visual Studio templates add references to the prebuilt versions of ANGLE that have been installed on your computer. Before we can add ANGLE using nuget, we need remove these references from the Breakout project file.

1. Open **Breakout.sln** in the **CodeLabs/Workshops/Games/Module3-ANGLE/Source/Ex7/Begin** folder.  

1. Select **Debug x86** from the Project Configuration and Platform menus.

	![Configuring the build target](../../Images/ex2-debug-x64.PNG?raw=true "Configuring the build target")

	_Configuring the build target_

1. Right click on the Breakout project file and select Unload Project from the menu.

	![Unload project](../../Images/ex7-unload-project.png?raw=true "Unload project")

	_Unload project_

1. Right click on the Breakout project and select Edit

	![Edit project](../../Images/ex7-edit-project.png?raw=true "Edit project")

	_Edit project_

1. Search for the word Angle and remove the following lines from the .vcxproj file:

    ````XML
    <PropertyGroup Label="ANGLE">
        <AngleIncPath Condition=" '$(AngleIncPath)' == '' ">$(AngleRootPath)\include\</AngleIncPath>
        <AngleBinPath Condition=" '$(AngleBinPath)' == '' ">$(AngleRootPath)\winrt\10\src\$(Configuration)_$(Platform)\</AngleBinPath>
    </PropertyGroup>

    <ItemGroup Label="ANGLE">
        <None Include="$(AngleBinPath)libEGL.dll">
            <DeploymentContent>true</DeploymentContent>
        </None>
        <None Include="$(AngleBinPath)libGLESv2.dll">
            <DeploymentContent>true</DeploymentContent>
        </None>
    </ItemGroup>
    ````

1. Search for $(AngleIncPath) and carefully remove the following references without breaking the XML

    Modify all instances of:

    ````XML
        <AdditionalIncludeDirectories>$(AngleIncPath);$(ProjectDir);$(IntermediateOutputPath);%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
    ````
    to:
    ````XML
        <AdditionalIncludeDirectories>$(ProjectDir);$(IntermediateOutputPath);%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
    ````
    Also modify all instances of:

    ````XML
        <AdditionalDependencies>$(AngleBinPath)lib\libGLESv2.lib;$(AngleBinPath)lib\libEGL.lib;mincore.lib;%(AdditionalDependencies)</AdditionalDependencies>
    ````
    to:
    ````XML
    <AdditionalDependencies>mincore.lib;%(AdditionalDependencies)</AdditionalDependencies>
    ````

1. Right click on the Breakout project and reload the project

	![Reload project](../../Images/ex7-reload-project.png?raw=true "Reload project")

	_Reload project_


#### Task 2 - Adding ANGLE using NuGet ####

1. Right click on the project and select Manage NuGet Packages…

	![Reload project](../../Images/ex7-manage-nuget.png?raw=true "Reload project")

	_Reload project_


1. Click on Browse and then search for ANGLE.WindowsStore. Select ANGLE.WindowsStore in the search results, then click on Install

	![ANGLE NuGet package](../../Images/ex7-nuget-angle.png?raw=true "ANGLE NuGet package")

	_ANGLE NuGet package_

1. Close the Nuget Window. Press F5 to build your project. It should now look like the following:


	![Breakout App_](../../Images/ex2-breakout-app.png?raw=true "Breakout App_")

	_Breakout App_

#### Discussion ####

You have now modified the Breakout project to use NuGet to add ANGLE to the project. This is the **recommended** method for adding ANGLE to a Visual Studio project.

Please note that the above changes will also need to be performed on the soil project as it also needs to access the ANGLE header files. Repeat the above steps to modify the soil project.


#### Next ####

This completes the ANGLE workshop.

- Return to [Start](../../README.md)
