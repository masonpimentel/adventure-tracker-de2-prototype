# CPEN391Project

High-level design docs can be found here: https://drive.google.com/folderview?id=0By6j1C4eNWHzQkhsV1RwN21uSjQ&usp=sharing

Notes on project management:

1. Because there are many generated files in Quartus, we should just delete our local copies and re-clone the entire repo.

2. I added software/ to the .gitignore so that there is no overlap when we push our changes to the files we have locally for Eclipse. So when we pull the repo, there won't be project files with the same name already there. 

For example, if I want to get a C file Mason worked on running on my project...

	1. Make a local directory where the repo will be cloned to
	2. Clone the repo
	3. Program your DE2 
	4. In Eclipse, you'll have your own workspace. But when you add a project (from BSP) a directory in Cpen391Project\QuartusProject\NIOS_II_System_391_For_Students\software will be created. This will only be local - since it is ignored by git, you don't have to worry about these directories overlapping with others
	5. Get the C file(s) you want from \Cpen391Project\SoftwareFiles 
	6. Add those file(s) to your own Cpen391Project\QuartusProject\NIOS_II_System_391_For_Students\software directory
	7. Modify your Quartus, C files as desired
	8. When it's time to commit, add all the affected files to \Cpen391Project\SoftwareFiles
	9. This should allow you to make changes to the Quartus project files, and push your C files as well  

