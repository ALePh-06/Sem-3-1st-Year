DROP TABLE IF EXISTS Portfolio;
DROP TABLE IF EXISTS Users;
DROP TABLE IF EXISTS Provider;
DROP TABLE IF EXISTS CloudAccount;
DROP TABLE IF EXISTS CloudResource;
DROP TABLE IF EXISTS Vulnerability;
DROP TABLE IF EXISTS SecurityScan;
DROP TABLE IF EXISTS Report;
DROP TABLE IF EXISTS RemedialPlan;
DROP TABLE IF EXISTS ProgressTracker;

-- Create table Portfolio
CREATE TABLE IF NOT EXISTS Portfolio (
    Portfolio_ID INT PRIMARY KEY NOT NULL,
    Company_Name VARCHAR(30),
    Industry VARCHAR(20)
);

-- Create table Users
CREATE TABLE IF NOT EXISTS Users (
    User_ID VARCHAR(10) PRIMARY KEY NOT NULL,
    Portfolio_ID INT(10),
    CONSTRAINT fk_Portfolio FOREIGN KEY (Portfolio_ID) REFERENCES Portfolio(Portfolio_ID),
    First_Name VARCHAR(15),
    Last_Name VARCHAR(15),
    Email VARCHAR(50),
    Phone_Number VARCHAR(15),
    User_Role VARCHAR(20)
);

-- Create table Provider
CREATE TABLE IF NOT EXISTS Provider (
    Provider_ID VARCHAR(10) PRIMARY KEY NOT NULL,
    Provider_Name VARCHAR(15),
    Provider_Type VARCHAR(15),
    Contact_Email VARCHAR(50),
    Website VARCHAR(30)
);

-- Create table Cloud Account
CREATE TABLE IF NOT EXISTS CloudAccount (
    Account_ID VARCHAR(15) PRIMARY KEY NOT NULL,
    User_ID VARCHAR(10),
    CONSTRAINT fk_User FOREIGN KEY (User_ID) REFERENCES Users(User_ID),
    Provider_ID VARCHAR(10),
    CONSTRAINT fk_Provider FOREIGN KEY (Provider_ID) REFERENCES Provider(Provider_ID),
    Account_Name VARCHAR(20),
    Account_Type VARCHAR(15),
    Status VARCHAR(10),
    Created_Date DATE
);

-- Create table Cloud Resource
CREATE TABLE IF NOT EXISTS CloudResource (
    Resource_ID VARCHAR(15) PRIMARY KEY NOT NULL,
    Account_ID VARCHAR(15),
    CONSTRAINT fk_CloudAccount FOREIGN KEY (Account_ID) REFERENCES CloudAccount(Account_ID),
    Resource_Type VARCHAR(15),
    Resource_Name VARCHAR(15),
    Status VARCHAR(10),
    Created_Date DATE
);

-- Create table Vulnerability
CREATE TABLE IF NOT EXISTS Vulnerability (
    Vulnerability_ID VARCHAR(15) PRIMARY KEY NOT NULL,
    Provider_ID VARCHAR(10),
    CONSTRAINT fk_Provider FOREIGN KEY (Provider_ID) REFERENCES Provider(Provider_ID),
    Vulnerability_Type VARCHAR(15),
    Vulnerability_Details VARCHAR(100),
    Severity_Level VARCHAR(10),
    Last_Updated DATE
);

-- Create table Security Scan
CREATE TABLE IF NOT EXISTS SecurityScan (
    Scan_ID VARCHAR(15) PRIMARY KEY NOT NULL,
    Resource_ID VARCHAR(15),
    CONSTRAINT fk_CloudResource FOREIGN KEY (Resource_ID) REFERENCES CloudResource(Resource_ID),
    Vulnerability_ID VARCHAR(15),
    CONSTRAINT fk_Vulnerability FOREIGN KEY (Vulnerability_ID) REFERENCES Vulnerability(Vulnerability_ID),
    Scan_Date DATE,
    Status VARCHAR(10)
);

-- Create table Report
CREATE TABLE IF NOT EXISTS Report (
    Report_ID VARCHAR(15) PRIMARY KEY NOT NULL,
    Scan_ID VARCHAR(15),
    CONSTRAINT fk_SecurityScan FOREIGN KEY (Scan_ID) REFERENCES SecurityScan(Scan_ID),
    Report_Date DATE,
    Report_Detail VARCHAR(100),
    Risk_Summary VARCHAR(50)
);

-- Create table Remedial Plan
CREATE TABLE IF NOT EXISTS RemedialPlan (
    Plan_ID VARCHAR(15) PRIMARY KEY NOT NULL,
    Report_ID VARCHAR(15),
    CONSTRAINT fk_Report FOREIGN KEY (Report_ID) REFERENCES Report(Report_ID),
    Plan_Name VARCHAR(20),
    Created_Date DATE,
    Plan_Status VARCHAR(10)
);

-- Create table Progress Tracker
CREATE TABLE IF NOT EXISTS ProgressTracker (
    Progress_ID VARCHAR(15) PRIMARY KEY NOT NULL,
    Plan_ID VARCHAR(15),
    CONSTRAINT fk_RemedialPlan FOREIGN KEY (Plan_ID) REFERENCES RemedialPlan(Plan_ID),
    Solution_Detail VARCHAR(150),
    Tracker_Status VARCHAR(10),
    Last_Updated DATE
);


--(YEAP) Create audit table for trigger
CREATE TABLE AuditLog (
    Audit_ID INT AUTO_INCREMENT PRIMARY KEY,
    Table_Name VARCHAR(30),
    Action_Type VARCHAR(10),   -- INSERT, UPDATE, DELETE
    Record_ID VARCHAR(20),
    Changed_By VARCHAR(50),
    Change_Date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    Change_Detail VARCHAR(200)
);
