#include "../include/PlotMaPMTData.h"

#include "TFile.h"

#include <filesystem>

int main()
{
    // Make sure the directory for storing pdfs is created
    try{
        std::filesystem::create_directory("../output");
    }
    catch (const std::filesystem::filesystem_error& e){
        std::cerr << e.what() << std::endl;
        return 1;
    }

    // Declare and initialise useful variables
    std::string quadrant = "";
    bool rotation = false;
    bool analyseRun = true;
    bool selectOption = true;
    int analysisOption = 999;
    std::string side; 
    std::string pdfName;
    int runNo;
    std::vector<int> highCountEvents = {};
    std::vector<int> eventsToPlot;
    int eventToPlot;

    while (analyseRun)
    {
        std::cout << "Provide number of the run to analyse " << std::endl;
        std::cin >> runNo;
        while (std::cin.fail()){
            std::cout << "Invalid option, provide run number as integer" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> runNo;
        }


        //Open ROOT files and retrieve trees
        //Left arm
        std::string inputnameL = std::string("../data/Run") + std::to_string(runNo) + std::string("/out-feb-45F6379E-000") + std::to_string(runNo) + std::string(".root");
        TFile* fL = new TFile(inputnameL.data(), "read");
        if (!fL || fL->IsZombie()){
            std::cerr << "Cannot open file or file does not exist" << std::endl;
            return 1;
        }
        TTree* tL = (TTree*)fL->Get("mdfTree");
        if (!tL){
            std::cerr << "mdfTree not found in file" << std::endl;
            return 1;
        }

        //Right arm 
        std::string inputnameR = std::string("../data/Run") + std::to_string(runNo) + std::string("/out-feb-4944044F-000") + std::to_string(runNo) + std::string(".root");
        TFile* fR = new TFile(inputnameR.data(), "read");
        if (!fR || fR->IsZombie()){
            std::cerr << "Cannot open file or file does not exist" << std::endl;
            return 1;
        }
        TTree* tR = (TTree*)fR->Get("mdfTree");
        if (!tL){
            std::cerr << "mdfTree not found in file" << std::endl;
            return 1;
        }

        selectOption = true;
        while (selectOption)
        {
            std::cout << "Select analysis option for run " << std::to_string(runNo) << " (-1/0/1/2/3/4/5/6)" << std::endl;
            std::cout << "1. 1D histogram of the number of hits in MaPMT for the selected side of the setup" << std::endl;
            std::cout << "2. 1D histograms of ADC counts for each anode separately for the selected side of the setup" << std::endl;
            std::cout << "3. 2D histogram of the mean ADC counts in anodes in the run for the selected side of the setup" << std::endl;
            std::cout << "4. Print out the numbers of high-count events for the selected setup side" << std::endl;
            std::cout << "5. 2D histograms of ADC counts for high-count events for the selected setup side" << std::endl;
            std::cout << "6. 2D histograms of ADC counts for user-selected events for both sides of the setup" << std::endl;
            std::cout << "0. Analyse another run" << std::endl;
            std::cout << "-1. Exit the program" << std::endl;

            std::cin >> analysisOption;

            while (std::cin.fail()){
                std::cout << "Invalid option, provide one of the following: -1, 0, 1, 2, 3, 4, 5, 6" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> analysisOption;
            }
            switch (analysisOption)
            {
                case 1: 
                    std::cout << "Select setup side (left/right)" << std::endl;
                    std::cin >> side;
                    std::cout << "Select quadrant (A/B/C/D)" << std::endl;
                    std::cin >> quadrant;
                    pdfName = "../output/hits_" + side + "_q" + quadrant + "_r" + std::to_string(runNo) + ".pdf";
                    if (side == "left")
                        PlotHits1D(tL, quadrant, pdfName);
                    else if (side == "right")
                        PlotHits1D(tR, quadrant, pdfName);
                    else {
                        std::cout << side << " is not a valid side, exiting..." << std::endl;
                    }
                    break;
                case 2: 
                    std::cout << "Select setup side (left/right)" << std::endl;
                    std::cin >> side;
                    std::cout << "Select quadrant (A/B/C/D)" << std::endl;
                    std::cin >> quadrant;
                    pdfName = "../output/anode1DADCcounts_" + side + "_q" + quadrant + "_r" + std::to_string(runNo) + ".pdf"; 
                    // setup has to be rotated on the right in order to fit inside the climate chamber 
                    if (side == "left"){
                        rotation = false;
                        PlotAnodeHistogram1D(tL, quadrant, rotation, pdfName);
                    }
                    else if (side == "right"){
                        rotation = true;
                        PlotAnodeHistogram1D(tR, quadrant, rotation, pdfName);
                    }
                    else {
                        std::cout << side << " is not a valid side, exiting..." << std::endl;
                        exit(0);
                    }
                    break;
                case 3:
                    std::cout << "Select setup side (left/right)" << std::endl;
                    std::cin >> side;
                    std::cout << "Select quadrant (A/B/C/D)" << std::endl;
                    std::cin >> quadrant;
                    pdfName = "../output/meanADCcounts_" + side + "_q" + quadrant + "_r" + std::to_string(runNo) + ".pdf";
                    // setup has to be rotated on the right in order to fit inside the climate chamber 
                    if (side == "left"){
                        rotation = false;
                        PlotMeanADCcounts2D(tL, quadrant, rotation, pdfName);
                    }
                    else if (side == "right"){
                        rotation = true;
                        PlotMeanADCcounts2D(tR, quadrant, rotation, pdfName);
                    }
                    else {
                        std::cout << side << " is not a valid side, exiting..." << std::endl;
                        exit(0);
                    }
                    break;
                case 4: 
                    std::cout << "Select setup side (left/right)" << std::endl;
                    std::cin >> side;
                    std::cout << "Select quadrant (A/B/C/D)" << std::endl;
                    std::cin >> quadrant;
                    if (side == "left"){
                        rotation = false;
                        highCountEvents = PrintHighCountEventsNumbers(tL, quadrant, rotation);
                    }
                    else if (side == "right"){
                        rotation = true;
                        highCountEvents = PrintHighCountEventsNumbers(tR, quadrant, rotation);
                    }
                    else {
                        std::cout << side << " is not a valid side, exiting..." << std::endl;
                        exit(0);
                    }
                    for (auto hce : highCountEvents){
                        std::cout << hce << "\t";
                    }
                    std::cout << std::endl;
                    break;
                case 5: 
                    std::cout << "Select setup side (left/right)" << std::endl;
                    std::cin >> side;
                    std::cout << "Select quadrant (A/B/C/D)" << std::endl;
                    std::cin >> quadrant;
                    if (side == "left"){
                        rotation = false;
                        highCountEvents = PrintHighCountEventsNumbers(tL, quadrant, rotation);
                        for (auto hce : highCountEvents){
                            pdfName = "../output/single_event_2D_ev" + std::to_string(hce) + "_left_q" + quadrant + "_r" + std::to_string(runNo) + ".pdf";
                            PlotEvent2D(tL, quadrant, rotation, hce, pdfName);
                        }
                    }
                    else if (side == "right"){
                        rotation = true;
                        highCountEvents = PrintHighCountEventsNumbers(tR, quadrant, rotation);
                        for (auto hce : highCountEvents){
                            pdfName = "../output/single_event_2D_ev" + std::to_string(hce) + "_right_q" + quadrant + "_r" + std::to_string(runNo) + ".pdf";
                            PlotEvent2D(tR, quadrant, rotation, hce, pdfName);
                        }
                    }
                    else {
                        std::cout << side << " is not a valid side, exiting..." << std::endl;
                        exit(0);
                    }
                    break;
                case 6:
                    std::cout << "Provide number(s) of the event(s) to plot (-1 to end)" << std::endl;
                    while ((std::cin >> eventToPlot) && (eventToPlot != -1))
                        eventsToPlot.push_back(eventToPlot);
                    for (auto evno : eventsToPlot){
                        std::cout << "Plotting 2D hist of ADC counts for event: " << evno << std::endl;
                        pdfName = "../output/single_event_2D_ev" + std::to_string(evno) + "_r" + std::to_string(runNo) + ".pdf";
                        PlotSingleEvent2DLR(tL, tR, evno, pdfName);
                    }
                    break;
                
                case 0:
                    selectOption = false;
                    break;
                case -1:
                    exit(0);
                default:
                    std::cout << "Not a valid option" << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
            }
        }
        fL->Close();
        fR->Close();
    }

    return 0;
}