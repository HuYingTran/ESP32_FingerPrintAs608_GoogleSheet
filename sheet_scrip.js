var sheet_id = "1u8VTG0Kuv-sl8rhMm7leTuXQJc8ndmMLlcBCc6uB350";
var sheet_name;
var col_id = -1;
var col_matric = -1;
var col_name = -1;
var col_level = -1;
var col_department = -1;
var col_state = -1;
var col_time = -1;
var output;

function getColumnIndex(sheet) {
    var headers = sheet.getRange(1, 1, 1, sheet.getLastColumn()).getValues()[0];
    
    for (var i = 0; i < headers.length; i++) {
      if (headers[i] === "ID") {
        col_id = i+1;  // find colum id
      }
      if (headers[i] === "Matric Number") {
        col_matric = i+1;  // find colum id
      }
      if (headers[i] === "Name") {
        col_name = i+1;  // find colum name
      }
      if (headers[i] === "Level") {
        col_level = i+1;  // find colum level
      }
      if (headers[i] === "Department") {
        col_department = i+1;  // find colum name
      }
      if (headers[i] === "State") {
        col_state = i+1;  // find colum state
      }
      if (headers[i] === "Time") {
        col_time = i+1;  // find colum time
      }
    }
}

function write_data(sheet, id, matric, name, level, department, state, time){
    getColumnIndex(sheet);
    var data = sheet.getRange("A:A").getValues(); // Get data in sheet
    for (var i = 1; i < data.length; i++) { // Start in row 2, break row 1 this is header
        if (data[i] == id) {
            // sheet.getRange(i + 1, col_matric).setValue(matric);
            // sheet.getRange(i + 1, col_name).setValue(name);
            // sheet.getRange(i + 1, col_level).setValue(level);
            // sheet.getRange(i + 1, col_department).setValue(department);
            sheet.getRange(i + 1, col_state).setValue(state);
            sheet.getRange(i + 1, col_time).setValue(time);

            output_matric = sheet.getRange(i + 1, col_matric).getValue();
            output_name = sheet.getRange(i + 1, col_name).getValue();
            output_level = sheet.getRange(i + 1, col_level).getValue();
            output_department = sheet.getRange(i + 1, col_department).getValue();
            return; // return when updated data
        }
    }
    // if no data user then creat new line data
    var lastRow = sheet.getLastRow() + 1;
    sheet.getRange(lastRow, col_id).setValue(id);
    sheet.getRange(lastRow, col_matric).setValue(matric);
    sheet.getRange(lastRow, col_name).setValue(name);
    sheet.getRange(lastRow, col_level).setValue(level);
    sheet.getRange(lastRow, col_department).setValue(department);
    sheet.getRange(lastRow, col_state).setValue(state);
    sheet.getRange(lastRow, col_time).setValue(time);
}

function delete_id(sheet, id) {
  var data = sheet.getRange("A:A").getValues();
  for (var i = 1; i < data.length; i++) {
    if (data[i] == id) {
      sheet.deleteRow(i + 1); // Delete colt
    }
  }
}

function doGet(e) {
    // Read var in url http get
    var id = e.parameter.id;
    var matric = e.parameter.matricNumber;
    var name = e.parameter.name;
    var level = e.parameter.level;
    var department = e.parameter.department;
    var state = e.parameter.state;
    var time = e.parameter.time;
    var has_delete = e.parameter.delete;

    var ss = SpreadsheetApp.openById(sheet_id);

    // Check sheet Home craeted
    var sheet_home = ss.getSheetByName("Home");

    if (has_delete === "true") {
      delete_id(sheet_home, id);
    } else
    {
        if(!sheet_home){
            sheet_home = ss.insertSheet("Home");
            sheet_home.getRange(1, 1, 1, 7).setValues([["ID", "Matric Number", "Name", "Level", "Department","State", "Time"]]); // creat header for sheet Home
        }
        write_data(sheet_home, id, matric, name, level, department, state, time);  //Update state in sheet Home

        // Delete default sheet_1
        var sheet_1 = ss.getSheetByName("Sheet1");
        if (sheet_1) {
            ss.deleteSheet(sheet_1);
        }

        // Check sheet for month created
        var sheetName = e.parameter.sheetName;
        if (sheetName && sheetName !== "Home") {
            sheet_name = ss.getSheetByName(sheetName);
            if (!sheet_name) {
                sheet_name = ss.insertSheet(sheetName); // Creat sheet month
                sheet_name.getRange(1, 1, 1, 7).setValues([["ID", "Matric Number", "Name", "Level", "Department","State", "Time"]]);
            }
            sheet_name.appendRow([id, output_matric, output_name, output_level, output_department, state, time]); // Write new line data
        }
    }
    return ContentService.createTextOutput(output_name.toString());
}
