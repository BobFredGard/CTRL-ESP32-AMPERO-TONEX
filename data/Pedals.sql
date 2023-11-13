DROP TABLE IF EXISTS "stomps";
CREATE TABLE "stomps" ("stomps_id" INTEGER PRIMARY KEY AUTOINCREMENT);

ALTER TABLE "stomps"	ADD COLUMN "1"	 tinyint DEFAULT "0"; /* Slot 1 Choice */
ALTER TABLE "stomps"	ADD COLUMN "2"	 tinyint DEFAULT "0"; /* Slot 2 Choice */
ALTER TABLE "stomps"	ADD COLUMN "3"	 tinyint DEFAULT "0"; /* Slot 3 Choice */
ALTER TABLE "stomps"	ADD COLUMN "4"	 tinyint DEFAULT "0"; /* Slot 4 Choice */
ALTER TABLE "stomps"	ADD COLUMN "5"	 tinyint DEFAULT "0"; /* Slot 5 Choice */
ALTER TABLE "stomps"	ADD COLUMN "6"	 tinyint DEFAULT "0"; /* Slot 6 Choice */

ALTER TABLE "stomps"	ADD COLUMN "7"	 tinyint DEFAULT "100"; /* Volume */
ALTER TABLE "stomps"	ADD COLUMN "8"  tinyint DEFAULT "0"; /* Stéréoisation Patch */
ALTER TABLE "stomps"	ADD COLUMN "9"	 tinyint DEFAULT "0"; /* Stéréoisation P5 */
ALTER TABLE "stomps"	ADD COLUMN "10" tinyint DEFAULT "0"; /* Stéréoisation P6 */
ALTER TABLE "stomps"	ADD COLUMN "11" tinyint DEFAULT "127"; /* Param 1 */
ALTER TABLE "stomps"	ADD COLUMN "12" tinyint DEFAULT "127"; /* Param 2 */
ALTER TABLE "stomps"	ADD COLUMN "13" tinyint DEFAULT "127"; /* Param 3 */

ALTER TABLE "stomps"	ADD COLUMN "14" tinyint DEFAULT "0"; /* Slot 1 state */
ALTER TABLE "stomps"	ADD COLUMN "15" tinyint DEFAULT "0"; /* Slot 2 state */
ALTER TABLE "stomps"	ADD COLUMN "16" tinyint DEFAULT "0"; /* Slot 3 state */
ALTER TABLE "stomps"	ADD COLUMN "17" tinyint DEFAULT "0"; /* Slot 4 state */
ALTER TABLE "stomps"	ADD COLUMN "18" tinyint DEFAULT "0"; /* Slot 5 state */
ALTER TABLE "stomps"	ADD COLUMN "19" tinyint DEFAULT "0"; /* Slot 6 state */

ALTER TABLE "stomps"	ADD COLUMN "20" tinyint DEFAULT "1"; /* Name Bank */
ALTER TABLE "stomps"	ADD COLUMN "21" tinyint DEFAULT "1"; /* Name Patch */

ALTER TABLE "stomps"	ADD COLUMN "patch" smallint DEFAULT "290"; /* Patch */