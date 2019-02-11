de PC vers Arduino (UDP)

XXXYYYYY YYYYYYYY (8 bits)

XXX : commandes
 - 0b000 demande de statut (veille ou éveil ?)
  YYYYYY YYYYYYYY : 0x00 veille | 0x01 réveille

 - 0b001 set statut -> possible de broadcast
  YYYYYY YYYYYYYY : 0x00

 - 0b010 envoie du nombre de trames en 1 sec dans les Y
  YYYYYY YYYYYYYY : puissance de commande (0% - 100%)

 - 0b011, 0b100, 0b101, 0b110, 0b111 reservépour une utilisation future


de Arduino vers PC (TCP)

XXXYYYYY YYYYYYYY (8 bits)

XXX : code réponse
 - 0b000 réponse à demande de statut -> YYYYY = 0b00001 (éveille) | 0b00000 (veille)
 - 0b001 ACK envoie du nombre de trames par sec -> YYYYY = 0b00000
