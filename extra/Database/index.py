import pyodbc
import os
from dataclasses import dataclass

@dataclass
class Migration:
    Year: int
    Month: int
    Day: int
    ID: int
    Name: str

for item in os.listdir("../patch/"):
    itemList = item.split('_')
    localMigration: Migration = Migration(int(itemList[0]), int(itemList[1]), int(itemList[2]), int(itemList[3]), itemList[4].split('.')[0])
    print("Running migration: " + localMigration.Name)