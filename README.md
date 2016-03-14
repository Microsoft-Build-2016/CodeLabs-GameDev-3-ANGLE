<a name="HOLTop" />
# Porting your OpenGL ES 2.0 Game to Windows 10 using ANGLE #

---

<a name="Overview" />
## Overview ##

With the introduction of the single, unified Windows 10 core and the Universal Windows Platform (UWP), one app can run across all Windows 10 devices. However, Windows 10 UWP does not support OpenGL or OpenGL ES. This module will show you how to port your existing OpenGL ES 2.0 game to Windows 10 UWP using [ANGLE](https://github.com/MSOpenTech/angle).  ANGLE allows Windows developers to seamlessly port their OpenGL ES apps to Windows 10 UWP by efficiently translating the OpenGL ES API into DirectX 11 API calls. During this hands on lab, you will be guided step by step in porting a simple OpenGL ES 2.0 game to a Windows 10 UWP app using ANGLE.

<a name="Objectives" />
### Objectives ###
In this module, you will learn how to:

- Create a new ANGLE project using a Visual Studio Template
- Integrate your game code with ANGLE
- Add touch events to your app
- Add keyboard events to your app
- Handle window resizing
- Add game resources to app
- Run your game on Windows 10 Phone

<a name="Prerequisites"></a>
### Prerequisites ###

The following is required to complete this module:

- [Visual Studio Community 2015][1] or greater.

[1]: https://www.visualstudio.com/products/visual-studio-community-vs

> **Note:** You can take advantage of the [Visual Studio Dev Essentials]( https://www.visualstudio.com/en-us/products/visual-studio-dev-essentials-vs.aspx) subscription in order to get everything you need to build and deploy your app on any platform.

<a name="Setup" />
### Setup ###
In order to run the exercises in this module, you will need to set up your environment first.

1. Open Windows Explorer and browse to this module's **Source** folder (CodeLabs/Workshops/Games/Module3-ANGLE/Source/).
1. Right-click on **Setup.cmd** and select **Run as administrator** to launch the setup process that will configure your environment and install the Visual Studio code snippets for this module.
1. If the User Account Control dialog box is shown, confirm the action to proceed.

> **Note:** Make sure you have checked all the dependencies for this module before running the setup.

<a name="CodeSnippets" />
### Using the Code Snippets ###

Throughout the module document, you will be instructed to insert code blocks. For your convenience, most of this code is provided as Visual Studio Code Snippets, which you can access from within Visual Studio 2015 to avoid having to add it manually. 

>**Note**: Each exercise is accompanied by a starting solution located in the **Begin** folder of the exercise that allows you to follow each exercise independently of the others. Please be aware that the code snippets that are added during an exercise are missing from these starting solutions and may not work until you have completed the exercise. Inside the source code for an exercise, you will also find an **End** folder containing a Visual Studio solution with the code that results from completing the steps in the corresponding exercise. You can use these solutions as guidance if you need additional help as you work through this module.

---

Estimated time to complete this module: **60 minutes**

>**Note:** When you first start Visual Studio, you must select one of the predefined settings collections. Each predefined collection is designed to match a particular development style and determines window layouts, editor behavior, IntelliSense code snippets, and dialog box options. The procedures in this module describe the actions necessary to accomplish a given task in Visual Studio when using the **General Development Settings** collection. If you choose a different settings collection for your development environment, there may be differences in the steps that you should take into account.

<a name="Exercises" />
## Exercises ##
This module includes the following exercises:

1. [Create a new ANGLE project using a Visual Studio Template](./Source/Ex1/README.md)
2. [Integrate your game code with ANGLE](./Source/Ex2/README.md)
3. [Resizing the Window](./Source/Ex3/README.md)
4. [Handling keyboard and pointer events](./Source/Ex4/README.md)

---

<a name="Summary" />
## Summary ##

By completing this module you should have learned:

1. how to create a new ANGLE project using a Visual Studio template.
2. how to integrate your game code with an ANGLE app.
3. how to handle window resizing.
4. how to handle keyboard and pointer events.

For more information on ANGLE, please refer to the [ANGLE wiki](https://github.com/MSOpenTech/angle/wiki) and the [ANGLE GitHub repo](https://github.com/MSOpenTech/angle).


> **Note:** You can take advantage of the [Visual Studio Dev Essentials]( https://www.visualstudio.com/en-us/products/visual-studio-dev-essentials-vs.aspx) subscription in order to get everything you need to build and deploy your app on any platform.

