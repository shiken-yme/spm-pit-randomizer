# Injecting a Rel Mod into an ISO
* Install [Wiimms ISO Tools](https://wit.wiimm.de/) if you haven't already
    * You may need to restart your computer after installation if the commands aren't working
* Open the folder with your SPM ISO in a command prompt
* Run the command `wit x ISONAMEHERE extracted`
    * Replace ISONAMEHERE with the name of your ISO
* Navigate to `extracted/DATA/files/`
    * This should contain folders like `a`, `eff` and `font`
* Create a folder named `mod`
* Copy the rel for the mod into this folder and rename it `mod.rel`
* In the same location as before, run the command `wit copy --align-files extracted mod.wbfs`
    * Feel free to delete the `extracted` folder after this is done if you want to save storage space
* Your ISO is now patched, return to the tutorial that linked you here for instructions on what to do next
