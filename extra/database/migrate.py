import pyodbc
import os
from dataclasses import dataclass
import configparser
config = configparser.ConfigParser()

def resolve_path(path: str):
    dirname = os.path.dirname(__file__)
    return os.path.join(dirname, path)

@dataclass
class Migration:
    Year: int
    Month: int
    Day: int
    ID: int
    Name: str

config.read(resolve_path("../../bin/conf/Database.ini"))

conn = pyodbc.connect('DRIVER={' + config["DBConnection"]["DBDRIVER"] +'};SERVER=' + config["DBConnection"]["DBHOST"] + ';UID=' + config["DBConnection"]["DBUSER"] + ';PWD=' + config["DBConnection"]["DBPASS"])
cursor = conn.cursor()

for item in os.listdir(resolve_path('./migrations')):
    itemList = item.split('_')
    if not len(itemList) > 3 or not "sql" in item: continue
    localMigration: Migration = Migration(int(itemList[0]), int(itemList[1]), int(itemList[2]), int(itemList[3]), itemList[4].split('.')[0])
    print("Running migration: " + localMigration.Name)
    file = open(item, "r")
    cursor.execute(file.read())