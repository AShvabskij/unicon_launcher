function Component() {}

Component.prototype.createOperations = function() {
    component.createOperations();
//  component.addOperation("Copy", "appunicon_launcher.exe", "@TargetDir@/appunicon_launcher.exe");
    // Add additional operations as needed for your files

    // Create desktop shortcut
    component.addOperation("CreateShortcut", "@TargetDir@/appunicon_launcher.exe", "@DesktopDir@/IRST.lnk",
                           "workingDirectory=@TargetDir@",
                           "iconPath=@TargetDir@/resources/logo.ico", "description=Open Intellegent Remote Support Tool");

    // Create Start Menu shortcut
    component.addOperation("CreateShortcut", "@TargetDir@/appunicon_launcher.exe", "@StartMenuDir@/IRST.lnk",
                           "workingDirectory=@TargetDir@",
                           "iconPath=@TargetDir@/resources/logo.ico", "description=Open Intellegent Remote Support Tool");
}
