/*
* Logger
*/

#include "Logger.hpp"

const int chipSelect = 10;
const char Logger::MODULE_LOGGER[] = "LOGGER";
const char Logger::NOM_FICHIER_LOGS[] = "rlogs.csv";


Logger::Logger() {}

void Logger::initSdcard() {
  toSdcard = false;

  if (!carteSd.init(SPI_HALF_SPEED, chipSelect)) {
    log(MODULE_LOGGER, "ERROR_SDCARD_INIT", "Erreur d'initialisation de la carte SD");
  }
  else {
    log(MODULE_LOGGER, "SDCARD_INITIALIZED", "Carte SD initialisée");
    switch (carteSd.type()) {
      case SD_CARD_TYPE_SD1:
        log(MODULE_LOGGER, "SDCARD_TYPE", "SD1");
        break;
      case SD_CARD_TYPE_SD2:
        log(MODULE_LOGGER, "SDCARD_TYPE", "SD2");
        break;
      case SD_CARD_TYPE_SDHC:
        log(MODULE_LOGGER, "SDCARD_TYPE", "SDHC");
        break;
      default:
        log(MODULE_LOGGER, "SDCARD_TYPE", "Unknown");
    }

    if (!volume.init(carteSd)) {
      log(MODULE_LOGGER, "ERROR_SDCARD_VOLUME", "Aucune partition FAT16/FAT32 trouvée.");
    }
    else {
      log(MODULE_LOGGER, "SDCARD_VOLUME_CLUSTERS", String(volume.clusterCount()));
      log(MODULE_LOGGER, "SDCARD_VOLUME_BLOCKS_PER_CLUSTER", String(volume.blocksPerCluster()));
      log(MODULE_LOGGER, "SDCARD_VOLUME_BLOCKS", String(volume.blocksPerCluster() * volume.clusterCount()));
      uint32_t volumesize;
      log(MODULE_LOGGER, "SDCARD_VOLUME_FAT_TYPE", String(volume.fatType()));
      volumesize = volume.blocksPerCluster(); // clusters are collections of blocks
      volumesize *= volume.clusterCount(); // we'll have a lot of clusters
      volumesize /= 2; // SD card blocks are always 512 bytes (2 blocks are 1KB)
      volumesize /= 1024;
      log(MODULE_LOGGER, "SDCARD_VOLUME_SIZE_MB", String(volumesize));

      // log(MODULE_LOGGER, "SDCARD_VOLUME_ALL_FILES", "");
      // root.openRoot(volume);
      // // list all files in the card with date and size
      // root.ls(LS_R | LS_DATE | LS_SIZE);
      // root.close();
    }
  }

  if (!SD.begin(chipSelect)) {
    log(MODULE_LOGGER, "ERROR_SDCARD_INIT", "Erreur d'initialisation de la carte SD");
  }
  else {
    log(MODULE_LOGGER, "SDCARD_INITIALIZED", "Carte SD initialisée");

    fichier = SD.open(NOM_FICHIER_LOGS, FILE_WRITE);
    if(fichier) {
      log(MODULE_LOGGER, "LOG_FILE_OPEN", "Fichier de logs ouvert");
      toSdcard = true;
    }
    else {
      log(MODULE_LOGGER, "ERROR_LOG_FILE", "Impossible d'ouvrir le fichier de logs");
    }
  }
}

// Force l'écriture du cache sur la carte SD
void Logger::flush() {
  if(fichier) {
    fichier.flush();
    log(MODULE_LOGGER, "SDCARD_FLUSH", "Ecriture des données du cache sur la carte SD");
  }
}

// Fonctions d'écriture des logs
void Logger::log(const char module[], const char message[], const char details[]) {
  if (toSerial || toSdcard || toUdp) {
    formatLog(strLog, module, message, details);
  }

  if(toSerial) Serial.println(strLog);
  if(toUdp) wifi->ecrireUdp(strLog);
  if(toSdcard) fichier.println(strLog);
}

void Logger::log(const char module[], const char message[], String details) {
  char str[LONGUEUR_MAX_CHAINE_CARACTERES];
  details.toCharArray(str, LONGUEUR_MAX_CHAINE_CARACTERES);
  log(module, message, str);
}


