#include <glib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "CMp4AnalyzerLogger.h"
#include "CMp4AnalyzerConfiguration.h"
#include "ui/CMp4InformationAnalyzerUi.h"

static const char* LogTag = "MAIN";

int main(int argc, char* argv[]) {
	CMp4AnalyzerUi mp4AnalyzerUi;

	if(false == CMp4AnalyzerConfiguration::getInstance()->loadConfiguration()) {
		printf("Failed to lad tester configuration\n");
		exit(1);
	}

	if(false == initializeTesterLogger()) {
		printf("Failed to initialize tester logger\n");
		exit(1);
	}

	TestNotice(LogTag, "Mp4 information analyzer start");

	gtk_init(&argc, &argv);

	mp4AnalyzerUi.createMainUi(&mp4AnalyzerUi);

	gtk_main();

	TestInfo(LogTag, "Running...");

	TestNotice(LogTag, "Mp4 information analyzer end");

	if(false == finalizeTesterLogger()) {
		TestError(LogTag, "Failed to finalize tester logger");
		exit(1);
	}

	return 0;
}

