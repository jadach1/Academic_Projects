Rem
Rem  NAME  Daniel Wilson, Jacob Adach, Isai Nahshunov
Rem  April 14 2014
Rem  DBS301 SECTION B
Rem  ASSIGNMENT 3
Rem
Rem-- CONNECT dbs301_161b01/CBwz6472@neptune 
Rem

SET FEEDBACK 1
SET NUMWIDTH 10
SET LINESIZE 180
--SET TRIMSPOOL ON
--SET TAB OFF
SET PAGESIZE 200
SET ECHO OFF

DROP TABLE Bed_Type CASCADE CONSTRAINTS;
DROP TABLE Room CASCADE CONSTRAINTS;
DROP TABLE Ward CASCADE CONSTRAINTS;
DROP TABLE Service_Cat CASCADE CONSTRAINTS;
DROP TABLE Specialty CASCADE CONSTRAINTS;
DROP TABLE Staff_Spec CASCADE CONSTRAINTS;
DROP TABLE service CASCADE CONSTRAINTS;
DROP TABLE Medicine CASCADE CONSTRAINTS;
DROP TABLE Staff CASCADE CONSTRAINTS;
DROP TABLE Bed CASCADE CONSTRAINTS;
DROP TABLE Patient CASCADE CONSTRAINTS;
DROP TABLE Note CASCADE CONSTRAINTS;
DROP TABLE Prescription CASCADE CONSTRAINTS;
DROP TABLE Treatment CASCADE CONSTRAINTS;
DROP VIEW patient_vu;

Prompt ****** Creating Tables ******

Create table Bed_Type (
BedTypeID char(4) constraint ID_Bedy_Type_Pk Primary Key,
BedtypeDescription varchar2(100) constraint Description_BedType_UK Unique);

Create table Room(
RoomId char(4) Constraint ID_Room_Pk Primary Key,
Room_Desc varchar2(30) Constraint Descritpion_Room_Uk Unique
);

CREATE TABLE Bed(
BedID char(4) 
CONSTRAINT bed_BedID_PK PRIMARY KEY,
Available char(1) NOT NULL
CONSTRAINT bed_Available_CK CHECK (UPPER(Available) IN ('Y', 'N')),
RoomID char(4) 
CONSTRAINT bed_RoomID_FK REFERENCES Room(RoomID),
BedTypeID char(4)
CONSTRAINT bed_BedTypeID_FK REFERENCES Bed_Type(BedTypeID));

CREATE TABLE Medicine (
MedicineID CHAR(6)
CONSTRAINT medicine_medID_PK PRIMARY KEY,
ScientName varchar2(20)
CONSTRAINT medicine_Sciname_UN UNIQUE,
CommonName varchar2(20) NOT NULL,
NormalDose varchar2(20) NOT NULL,
MedComment varchar2(500),
QuantOnStock number(5) NOT NULL
CONSTRAINT medicine_Quant_CK CHECK (QuantOnStock >=0),
UnitMeasure varchar2(3) NOT NULL);

CREATE TABLE Patient (
PatientID CHAR(6) 
CONSTRAINT patient_PatientID_PK PRIMARY KEY,
LastName varchar2(20) NOT NULL,
FirstName varchar2(20) NOT NULL,
MiddleName varchar2(20),
Address varchar2(20) NOT NULL,
DOB date NOT NULL,
PhoneNum varchar2(15) NOT NULL,
SSN varchar2(9)
CONSTRAINT patient_SSN_UN UNIQUE,
BedNum char(4)
CONSTRAINT patient_BedNum_FK REFERENCES Bed(BedID));

Create table Ward(
WardID char(4) Constraint ID_Ward_Pk Primary Key,
Ward_Name varchar(30) Not Null,
Location varchar(50) Not Null,
Ward_Phone varchar(15) Not Null
);

CREATE TABLE Staff (
StaffID CHAR(6)
CONSTRAINT staff_StaffID_PK PRIMARY KEY,
LastName varchar2(20) NOT NULL,
FirstName varchar2(20) NOT NULL,
MiddleName varchar2(20),
JobTitle varchar2(40) NOT NULL,
WorkPhone varchar2(15) NOT NULL,
Extension varchar2(4) NOT NULL,
SSN varchar2(9)
CONSTRAINT staff_SSN_UN UNIQUE,
HireDate date,
Salary number(8,2) 
CONSTRAINT staff_Salary_CK CHECK (salary >=0),
WageRate number (5,2)
CONSTRAINT staff_WageRate_CK CHECK (WageRate >=0),
LicenseNumeric number(8),
WardID char(4)
CONSTRAINT staff_WardID_FK REFERENCES Ward(WardID));

CREATE TABLE Note(
PatientID CHAR(6)
CONSTRAINT note_PatientID_FK REFERENCES Patient(PatientID),
NoteDate date,
Comments varchar2(150) NOT NULL,
StaffID CHAR(6)
CONSTRAINT note_StaffID_FK REFERENCES Staff(StaffID),
CONSTRAINT note_patientnote_PK PRIMARY KEY (PatientID, NoteDate));

Create table Service_Cat(
CategCode CHAR(6) Constraint CategoryCode_Service_Cat_PK Primary Key,
CategDesc varchar2(50) Constraint Description_Service_Cat_UK Unique
);

CREATE TABLE service(
ServiceID CHAR(6)
CONSTRAINT service_serID_PK PRIMARY KEY,
ServiceDescrip varchar2(100)
CONSTRAINT service_serDesc_UN UNIQUE,
Charge number(6,2) NOT NULL
CONSTRAINT service_Charge_CK CHECK(charge >=0),
ServiceComment varchar2(500),
CategCode CHAR(6)
CONSTRAINT service_CategCode_FK REFERENCES Service_cat(CategCode));

CREATE TABLE Prescription (
PrescriptionID CHAR(6)
CONSTRAINT prescription_PreID_PK PRIMARY KEY,
PrescriptionDate date NOT NULL,
Dosage varchar2(10) NOT NULL,
Instruction varchar2(500) NOT NULL,
PatientID CHAR(6) 
CONSTRAINT prescription_PatID_FK REFERENCES Patient(PatientID),
MedicineID CHAR(6) 
CONSTRAINT prescription_medID_FK REFERENCES Medicine(MedicineID),
StaffID CHAR(6)
CONSTRAINT prescription_StaffID_FK REFERENCES Staff(StaffID),
CONSTRAINT prescription_natkey_UN UNIQUE (MedicineID, PatientID, PrescriptionDate));

Create Table Specialty (
SpecCode Numeric(3) Constraint SpecCode_Specialty_PK Primary key,
SpecTitle varchar(50) Not Null,
HowAwarded char(1) Constraint HowAwarded_Specialty_CK
Check (upper(HowAwarded) in ('E','A','P'))
);

Create Table Staff_Spec(
SpecCode Numeric(3),
StaffId CHAR(6),
AwardDate Date Not Null,
Constraint SpecCode_Staff_Spec_FK Foreign Key(SpecCode)
References Specialty(SpecCode),
Constraint StaffId_Staff_Spic_FK Foreign Key(StaffId)
References Staff(StaffId),
Constraint StaffSpec_PK Primary KEY (SpecCode, StaffID)
);

Create table Treatment (
TreatID Char(6) CONSTRAINT TreatID_PK PRIMARY KEY,
TreatDate Date NOT NULL,
ActualCharge number(8,2) NOT NULL CONSTRAINT ActualCharge_CK CHECK (ActualCharge >=0), 
TreatComment varchar2(100),
PatientID CHAR(6) CONSTRAINT PatientID_FK1 REFERENCES Patient(PatientID),
StaffID CHAR(6) CONSTRAINT StaffID_FK2 REFERENCES Staff(StaffID),
ServiceID CHAR(6) CONSTRAINT ServiceID_FK3 REFERENCES Service(ServiceID),
CONSTRAINT treatment_natkey_UN UNIQUE(PatientID, StaffID, ServiceID, TreatDate)
);

Prompt ****** Inserting into Tables ******

Insert into Room values 
('RP01','Private Room');
Insert into Room values 
('RL01','Labratory');
Insert into Room values 
('RIC7','Intensive Care Unit');

Insert into Bed_Type values
('BT01','This is a Single Standard Bed');
Insert into Bed_Type values
('BT77','This is a Labratory Bed');
Insert into Bed_Type values
('BT92','This is an ICU bed');

Insert into Bed values
('B001','Y','RP01','BT01');
Insert into Bed values
('B077','N','RL01','BT77');
Insert into Bed values
('B092','N','RIC7','BT92');

Insert into Medicine values
('MAB001','Acetlsalicylic Acid','Aspirin','500mg','Do not consume with alcohol',643,'mg');
Insert into Medicine values
('MAB002','Acetaminophen','Tylenol','250mg','Do not overexceed 4 pills a day',435,'mg');
Insert into Medicine values
('MAB003','3-Methylporphine','Codeine','50mg','Do not exceed 3 pills a day',1450,'mg');

insert into Patient values
('PM0001','Gresham','Jonathon','Taves','45 Pond rd',TO_DATE('17-JUN-1987', 'dd-MON-yyyy'),'289-125-2341','234123421','B001');
insert into Patient values
('PM0002','Travis','Scott','Kirk','455 Highland Dr',TO_DATE('24-DEC-1956', 'dd-MON-yyyy'),'416-234-4567','123444555','B077');
insert into Patient values
('PF0003','Harding','Tonya',null,'111 Trey Dr',TO_DATE('02-SEP-1998', 'dd-MON-yyyy'),'647-555-5555','333444555','B092');

Insert into Ward values 
('GW01','General Ward','Hospital Temporary Treatments','416-234-1321');
Insert into Ward values 
('EW02','Emergency Ward','Hospital Emergency Room','416-234-3456');
Insert into Ward values 
('ICW3','Intensive Care Unit','The Intensive Care Unit','416-234-6677');

Insert into Staff values 
('SN0001','Gripel','Wendy','Chofsta','Nurse Practitioner','416-234-3344','2351','345666732',TO_DATE('07-FEB-1995', 'dd-MON-yyyy'), NULL, 30, 2345634,'GW01');
Insert into Staff values 
('SRN002','Thomas','Henry','Tobias','Registered Nurse Practitioner','416-234-3345','2341','342112345',TO_DATE('28-JUL-2001', 'dd-MON-yyyy'), 85000, null, 1345623,'ICW3');
Insert into Staff values 
('SD0003','Wykoviec','Felicia',null,'General Doctor Practitioner','416-234-5533','3344','576643322',TO_DATE('05-JAN-1988', 'dd-MON-yyyy'), 100000, null, 4533214,'GW01');

Insert into Note values 
('PM0001','11-Nov-15','Patient is admitted into hospital, broken leg sent for x-rays','SN0001');
Insert into Note values 
('PM0002','15-Mar-16','Patient admitted to hospital with severe trauma to the head','SRN002');
Insert into Note values 
('PF0003','11-Jan-16','Patient is admitted to hospital, brought by ambulance, unconcious and non responsive','SD0003');

Insert into Service_Cat values 
('CC0001','Less then 24 hours stay');
Insert into Service_Cat values 
('CCS002','Surgery: Less then 1 week stay');
Insert into Service_Cat values 
('CCIC03','ICU:  Time is unkown');

Insert into Service values
('SV0100','General Admission',75.00,'For Patients who are admitted for general needs. usually they will leave within 24 hours. nothing major','CC0001');
Insert into Service values
('SV0200','Patient Requires Surgery. will be operated on',5000.00,'A Patient is admitted and requires surgery, after the surgery the patient will need to remain in the hospital for a few days up to a week to recover','CCS002');
Insert into Service values
('SV0300','Patient is in Critical Condition',1500.00,'Patient arrives/develops a critical condition, patient will be placed in an ICU. consistently monitored,time of stay:unknown', 'CCIC03');

Insert into Prescription values
('PIP001',TO_DATE('23-Mar-16', 'dd-MON-yy'),'2G','Take tablet twice a day','PM0001','MAB001','SD0003');
Insert into Prescription values
('PIP002',TO_DATE('26-Mar-16', 'dd-MON-yy'),'500MG','Take tablet twice daily until finished','PM0001','MAB002','SD0003');
Insert into Prescription values
('PIP003',TO_DATE('02-Apr-16','dd-MON-yy'),'5G','Take 4 times daily until finished','PM0002','MAB003','SD0003');

Insert into Specialty values
(111,'Advanced First Aid','E');
Insert into Specialty values
(222,'Advanced CPR','E');
Insert into Specialty values
(333,'Ability to write Prescriptions','E');

Insert into Staff_Spec values
(111,'SN0001',TO_DATE('22-JAN-2008', 'dd-MON-yyyy'));
Insert into Staff_Spec values
(222,'SRN002',TO_DATE('16-JUN-2001', 'dd-MON-yyyy'));
Insert into Staff_Spec values
(333,'SD0003',TO_DATE('02-FEB-1991', 'dd-MON-yyyy'));

Insert into Treatment values 
('IPT001',TO_DATE('18-MAR-2016', 'dd-MON-yyyy'),250,'Patient Received General Check up and X rays','PM0001','SN0001','SV0100');
Insert into Treatment values 
('IPT002',TO_DATE('02-APR-2016', 'dd-MON-yyyy'),5500,'Patient underwent surgery for wound','PM0002','SRN002','SV0200');
Insert into Treatment values 
('IPT003',TO_DATE('02-NOV-2015', 'dd-MON-yyyy'), 24543,'Patient Remained in ICU for several months due to critical condition','PF0003','SD0003','SV0300');


PROMPT ****** Creating View ******
CREATE VIEW patient_vu
                        AS
                 SELECT    Patient.FirstName PFirst, Patient.MiddleName PMiddle, Patient.LastName PLast,
                           Treatment.TreatDate TreatDate, Treatment.ActualCharge TreatCharge, 
			   Service.Serviceid ServiceDesc, Staff.FirstName SFirst, Staff.MiddleName                                          
			   SMiddle, Staff.LastName SLast, Staff.JobTitle SJobTitle,
                           Ward.Ward_name Ward_name
                FROM Patient
                                    JOIN Treatment
                                    ON Treatment.PatientID = Patient.PatientID
                                    JOIN Service
                                    ON Service.ServiceID = Treatment.ServiceID
                                    JOIN Staff
                                    ON Staff.StaffID = Treatment.StaffID
                                    JOIN Ward
                                    ON Ward.WardID = Staff.WardID;

PROMPT ****** Display View ******
SELECT SLast AS "Last Name", SFirst AS "First Name", SJobTitle job, Ward_Name Ward, TO_CHAR(TreatCharge, 'fm$999999.99') Charge
FROM patient_vu
WHERE ServiceDesc = 'SV0100';





COMMIT;
