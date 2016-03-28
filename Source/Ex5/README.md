<a name="HOLTop" />
# Adding Resources to the Game #

---


<a name="Exercise5" />
### Exercise 5: Adding Resources to the Game ###

In this exercise, you will learn how to add and modify game resources. A game may have hundreds of resource files organized into a specific directory structure.
These files may include textures, shaders, text files, etc. In a cross platform game, these resource files will be located outside of your Visual Studio project directory.

It can be difficult to manually add all of these files to the Visual Studio project. A Windows 10 UWP app cannot simply have these resource files copied to the project's output directory. They need to be packaged as part of the Windows 10 UWP app and therefore they need to be handled by the build process.

Instead of manually adding potentially hundreds of files to our game's project file, we are going to use a property sheet to instruct the build process to correctly add the resources to the app package during the build.

In this exercise, you will complete the following tasks:

1. Learn how the resource files are added to the game.
1. Modify a game resource.

#### Task 1 - Adding Resources to the Game ####

1. Open **Breakout.sln** in the **CodeLabs/Workshops/Games/Module3-ANGLE/Source/Ex5/Begin** folder.  

1. Select **Debug x64** from the Project Configuration and Platform dropdowns.

	![Configuring the build target](../../Images/ex2-debug-x64.PNG?raw=true "Configuring the build target")

	_Configuring the build target_

1. Press **F5** to build and run the project. The app should look like the following.

	![No Resources](../../Images/ex5-no-resources.png?raw=true "No Resources")

	_No resources_

    The resource has been removed from the project and you should see a black window.

1. We will now add all of the resource files to our game. The resource files are located in **CodeLabs-GameDev-3-ANGLE\Source\Resources**.

 	![Resources Folder](../../Images/ex5-resources-folder.png?raw=true "Resources Folder")

	_Resources folder_

1. Instead of adding each resource file manually to our project, we are going to use a **[property sheet](./Begin/Breakout/resources.props)** to tell the project where to find the resource files. To add a property sheet, select **Property Manager** from the **View** menu. **Note:** You may need to select the **Property Manager** from **View ->Other Windows -> PropertyManger..** depending on how your copy of Visual Studio is configured.

	![Property Manager](../../Images/ex5-property-manager.png?raw=true "Property Manager")

	_Property Manager_

1. Right click the **Breakout** project and select **Add Existing Property Sheet...**.

	![Add Existing Property Sheet](../../Images/ex5-property-manager.png?raw=true "Add Existing Property Sheet")

	_Add existing property sheet_

1. Then select the file **resources.props** file in the folder **CodeLabs-GameDev-3-ANGLE\Source\Ex5\Begin\Breakout**.

	![Add resources.props](../../Images/ex5-resourceprops.png?raw=true "resources.props")

	_resources.props_

1. Close the Property Manager window.

	![Close Property Manager](../../Images/ex5-close-property-manager.png?raw=true "Close Property Manager")

	_Close Property Manager_

1. Press **F5** to build and run the project. The app should look like the following.

	![Breakout App with Resources](../../Images/ex4-breakout-app-input.png?raw=true "Breakout App with Resources")

	_Breakout App with Resources_

### Task 2 - Modifying Resources###

1. Now we are going to add a resource to our game. Copy the **awesomeface.png** file located at **CodeLabs-GameDev-3-ANGLE\Source\Ex5\Resources\textures\** to the **CodeLabs-GameDev-3-ANGLE\Source\Resources\textures\** folder.

1. Open the file **Source/Game.cpp** and change line 53.

	from:

	````C++
	ResourceManager::LoadTexture("textures/ball.png", GL_TRUE, "face");
	````

	to:

	````C++
	ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
	````

1. Save your work. Press **F5** to build and run your app. Your app should now look like this:

	![Exercise 5 Complete](../../Images/ex5-end.png?raw=true "Ex5 Complete")

	_Exercise 5 Complete_

#### Discussion ####

You have now learned how to add and modify the resources for your game. By placing the resources for your game outside of the Visual Studio project folder it will be easier to make your game cross platform. By using a property sheet to automatically add the game resources, you won't have to manually add potentially hundreds of resource files to your game project.


#### Next ####

- Continue on to [Exercise 6: Windows Phone](../../Source/Ex6/README.md)
- Return to [Start](../../README.md)
