function Component() {
    // This function will execute when the installation starts
    installer.setValue("TargetDirectoryInUse", "false");
}

Component.prototype.createOperations = function() {
    // Proceed with default operations
    component.createOperations();

    var targetDir = installer.value("TargetDir");
    var licenseFilePath = targetDir + "\config\license.dat";
    var backupLicenseFilePath = targetDir + "config\backup_license.dat"; // Backup location

    // Check if the directory exists by trying to get its content

     var fileExists = installer.fileExists(licenseFilePath); // Check if the file exists

     // if (fileExists) {
     //    component.addOperation("Copy", licenseFilePath, backupLicenseFilePath);
     // }


    // Create desktop shortcut
    component.addOperation("CreateShortcut", "@TargetDir@/appunicon_launcher.exe", "@DesktopDir@/IRST.lnk",
                           "workingDirectory=@TargetDir@",
                           "iconPath=@TargetDir@/resources/logo.ico", "description=Open Intellegent Remote Support Tool");

    // Create Start Menu shortcut
    component.addOperation("CreateShortcut", "@TargetDir@/appunicon_launcher.exe", "@StartMenuDir@/IRST.lnk",
                           "workingDirectory=@TargetDir@",
                           "iconPath=@TargetDir@/resources/logo.ico", "description=Open Intellegent Remote Support Tool");
};
