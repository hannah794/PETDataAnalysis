#include "TBranch.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TStyle.h"

#include <map>
#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <numeric>

// Note: at the moment, none of the ADC counts histograms are corrected for the pedestal value

// Returns the pad number based on the MaPMT anode number so that for plotting 
// the plot for the selected anode is shown in the place as where the anode is 
// looking at the front of the MaPMT baseboard
int GetPadCoords(std::string quadrant, int anodeno, bool rotation)
{
    std::map<int, int> anode2PadMap;
    int pad = -1;
    if ((quadrant == "A" && !rotation) || (quadrant == "C" && rotation))
        pad =  65 - anodeno;
    else if ((quadrant == "D" && rotation) || (quadrant == "B" && !rotation))
    {
        anode2PadMap = {{1 ,57}, {2 ,49}, {3 ,41}, {4 ,33}, {5 ,25}, {6 ,17}, {7 ,9}, {8 ,1}, {9 ,58}, {10 ,50}, {11 ,42}, {12 ,34}, {13 ,26}, {14 ,18}, {15 ,10}, {16 ,2}, {17 ,59}, {18 ,51}, {19 ,43}, {20 ,35}, {21 ,27}, {22 ,19}, {23 ,11}, {24 ,3}, {25 ,60}, {26 ,52}, {27 ,44}, {28 ,36}, {29 ,28}, {30 ,20}, {31 ,12}, {32 ,4}, {33 ,61}, {34 ,53}, {35 ,45}, {36 ,37}, {37 ,29}, {38 ,21}, {39 ,13}, {40 ,5}, {41 ,62}, {42 ,54}, {43 ,46}, {44 ,38}, {45 ,30}, {46 ,22}, {47 ,14}, {48 ,6}, {49 ,63}, {50 ,55}, {51 ,47}, {52 ,39}, {53 ,31}, {54 ,23}, {55 ,15}, {56 ,7}, {57 ,64}, {58 ,56}, {59 ,48}, {60 ,40}, {61 ,32}, {62 ,24}, {63 ,16}, {64 ,8}};
        pad =  anode2PadMap.at(anodeno);
    }
    else if ((quadrant == "C" && !rotation) || (quadrant == "A" && rotation))
    {
        pad = anodeno;
    }
    else if ((quadrant == "D" && !rotation) || (quadrant == "B" && rotation))
    {
        anode2PadMap = {{1 ,8}, {2 ,16}, {3 ,24}, {4 ,32}, {5 ,40}, {6 ,48}, {7 ,56}, {8 ,64}, {9 ,7}, {10 ,15}, {11 ,23}, {12 ,31}, {13 ,39}, {14 ,47}, {15 ,55}, {16 ,63}, {17 ,6}, {18 ,14}, {19 ,22}, {20 ,30}, {21 ,38}, {22 ,46}, {23 ,54}, {24 ,62}, {25 ,5}, {26 ,13}, {27 ,21}, {28 ,29}, {29 ,37}, {30 ,45}, {31 ,53}, {32 ,61}, {33 ,4}, {34 ,12}, {35 ,20}, {36 ,28}, {37 ,36}, {38 ,44}, {39 ,52}, {40 ,60}, {41 ,3}, {42 ,11}, {43 ,19}, {44 ,27}, {45 ,35}, {46 ,43}, {47 ,51}, {48 ,59}, {49 ,2}, {50 ,10}, {51 ,18}, {52 ,26}, {53 ,34}, {54 ,42}, {55 ,50}, {56 ,58}, {57 ,1}, {58 ,9}, {59 ,17}, {60 ,25}, {61 ,33}, {62 ,41}, {63 ,49}, {64 ,57}};
        pad =  anode2PadMap.at(anodeno);
    }
    return pad; 
}

// Returns the bin coordinates for a 2D plot where each bin corresponds to one of the 
// MaPMT anodes and is placed where the anode is looking at the front of the MaPMT 
// baseboard (taking into account the rotation)
// anodeno is from 1 to 64
std::pair<int, int> GetBinCoords(std::string quadrant, int anodeno, bool rotation)
{
    std::pair<int, int> coords = {-1, -1};
    
    if ((quadrant == "A" && !rotation) || (quadrant == "C" && rotation))
    {
        coords.first = 8 - ((anodeno - 1)%8);
        coords.second = std::floor((anodeno -1)/8) + 1;
    }
    else if ((quadrant == "B" && !rotation) || (quadrant == "D" && rotation))
    {
        coords.first = std::floor((anodeno-1)/8) + 1;
        coords.second = (anodeno-1)%8 + 1;
    }
    else if ((quadrant == "C"  && !rotation) || (quadrant == "A" && rotation))
    {
        coords.first = ((anodeno-1)%8) + 1;
        coords.second = 8 - std::floor((anodeno -1)/8);
    }
    else if ((quadrant == "D" && !rotation) || (quadrant == "B" && rotation))
    {
        coords.first = 8 - std::floor((anodeno -1)/8);
        coords.second = 8 - ((anodeno - 1)%8);
    }
    else 
    {   
        std::cout << "Incorrect quadrant identifier" << std::endl;
    }
    return coords;
}

// Returns the right branch name to access the ADC counts for particular quadrant
// It seems this is in contradiction to Silvia's documentation as quadrant A contents 
// correspond to PMT2 in the ROOT files while in her document it was supposed to be PMT1
// (maybe something changed in the mdf files over time?)
std::string GetADCBranchName(std::string quadrant)
{
    if (quadrant == "B" || quadrant == "D")
        return "B01_PMT1_ADC";
    else if (quadrant == "A" || quadrant == "C")
        return "B01_PMT2_ADC";
    else 
        std::cout << quadrant << " is not a valid quadrant name, exiting..." << std::endl;
        exit(0);
}

// Returns the right branch name to access the hits registered, for particular quadrant
// It seems this is in contradiction to Silvia's documentation as quadrant A contents 
// correspond to PMT2 in the ROOT files while in her document it was supposed to be PMT1 
// (maybe something changed in the mdf files over time?)
std::string GetHitsBranchName(std::string quadrant)
{
    if (quadrant == "B" || quadrant == "D")
        return "B01_PMT1Hits";
    else if (quadrant == "A" || quadrant == "C")
        return "B01_PMT2Hits";
    else 
        std::cout << quadrant << " is not a valid quadrant, exiting..." << std::endl;
        exit(0);
}

// 1D histogram of number of hits registered in the MaPMT
void PlotHits1D(TTree* mdfTree, std::string quadrant, std::string pdfName)
{
    std::string branchname = GetHitsBranchName(quadrant);
    std::cout << "Retrieving branch: " << branchname;
    
    short hits;
    TBranch* bHits = mdfTree->GetBranch(branchname.data());
    std::cout << bHits << std::endl;
    bHits->SetAddress(&hits);

    TH1F hHits = TH1F("hHits", "Hits", 2, 0, 2);
    for (int i=0; i<mdfTree->GetEntries(); ++i){
        mdfTree->GetEntry(i);
        hHits.Fill(hits);
    }

    TCanvas cHits = TCanvas("cHits", "", 800, 800);
    hHits.Draw("hist");
    cHits.SaveAs(pdfName.data());
}

// Same as above but for both sides and on predefined quadrants, i.e. A for left and D for right
void PlotHits1DLR(TTree* tL, TTree* tR, std::string pdfName)
{
    short hitsL;
    short hitsR;

    std::string branchnameL = GetHitsBranchName("A");
    std::string branchnameR = GetHitsBranchName("D");

    TBranch* bL = tL->GetBranch(branchnameL.data());
    bL->SetAddress(&hitsL);
    TBranch* bR = tR->GetBranch(branchnameR.data());
    bR->SetAddress(&hitsR);

    //It is possible that the number of hits in both sides does not match so need separate loops
    TH1F hHitsL = TH1F("hHitsL", "Hits", 2, 0, 2);
    TH1F hHitsR = TH1F("hHitsR", "Hits", 2, 0, 2);
    for (int i=0; i<bL->GetEntries(); ++i){
        tL->GetEntry(i);
        hHitsL.Fill(hitsL);
    }
    for (int i=0; i<bR->GetEntries(); ++i){
        tR->GetEntry(i);
        hHitsR.Fill(hitsR);
    }
    TCanvas cHitsLR = TCanvas("cHitsLR", "", 1600, 800);
    cHitsLR.Divide(2,1);
    cHitsLR.cd(1);
    hHitsL.Draw("hits");
    cHitsLR.cd(2);
    hHitsR.Draw("hits");
    cHitsLR.SaveAs(pdfName.data());
}

// 1D histograms of ADC counts in run for each of the 64 MaPMT anodes
// all on one canvas and placed in a way in which the anodes would be visible 
// when looking at the front of the MaPMT
void PlotAnodeHistogram1D(TTree* mdfTree, std::string quadrant, bool rotation, std::string pdfName)
{
    std::string branchname = GetADCBranchName(quadrant);

    short ADC[64];

    // std::cout << "Branchname = " << branchname << std::endl;
    TBranch* bADC = mdfTree->GetBranch(branchname.data());
    bADC->SetAddress(&ADC);

    
    int anode = 0;
    std::map<int, TH1F> hADCs;

    TH1F hADC = TH1F("hADC", "", 100, 0, 100);

    // Create and fill histograms
    for (int anode=1; anode<=64; ++anode){
        std::string name = std::string("hist_") + std::to_string(anode);
        std::string title = std::string("Anode = ") + std::to_string(anode);
        hADCs.insert({anode, TH1F(name.data(), title.data(), 100, 0, 500)});
        for (int i=0; i<mdfTree->GetEntries(); ++i){
            mdfTree->GetEntry(i);
            // std::cout << "Anode" << ADC[anode-1] << std::endl;
            hADCs.at(anode).Fill(ADC[anode-1]);
        }
    }
    
    // Draw the histograms on the corresponding pad
    int padno;
    TCanvas cAnode1D = TCanvas("cAnode1D", "", 3000, 3000); 
    cAnode1D.Divide(8, 8);
    for (int anode=1; anode<=64; ++anode)
    {
        cAnode1D.cd(GetPadCoords(quadrant, anode, rotation));
        gPad->SetLogy();
        hADCs.at(anode).Draw("hist");
        
    }
    
    cAnode1D.SaveAs(pdfName.data());
    
}

// 2D histogram showing the mean ADC counts in each of the 64 anodes of one MaPMT
// Looking at the mean makes sense only for a pedestal run as the distributions from 
// real data runs have multiple peaks
void PlotMeanADCcounts2D(TTree* mdfTree, std::string quadrant, bool rotation, std::string pdfName)
{
    std::string branchname = GetADCBranchName(quadrant);

    short ADC[64];

    TBranch* bADC = mdfTree->GetBranch(branchname.data());
    bADC->SetAddress(&ADC);

    std::map<int, TH1F> hADCs;

    TH1F hADC = TH1F("hADC", "", 100, 0, 500);

    TH2F hMeanADC = TH2F("hMeanADC", "Mean ADC counts", 8, 0, 8, 8, 0, 8);

    // Create and fill histograms
    for (int anode=1; anode<=64; ++anode){
        std::string name = std::string("hist_") + std::to_string(anode);
        std::string title = std::string("Anode = ") + std::to_string(anode);
        hADCs.insert({anode, TH1F(name.data(), title.data(), 100, 0, 500)});
        for (int i=0; i<mdfTree->GetEntries(); ++i){
            mdfTree->GetEntry(i);
            hADCs.at(anode).Fill(ADC[anode-1]);
        }
        std::pair<int, int> binno = GetBinCoords(quadrant, anode, rotation);
        hMeanADC.SetBinContent(binno.first, binno.second, hADCs.at(anode).GetMean());
    }
    
    // Draw the histograms on the corresponding pad
    int padno;
    TCanvas cMeanADC = TCanvas("cMeanADC", "Mean ADC counts", 1280, 1280); 
    hMeanADC.Draw("colz text");
    gStyle->SetOptStat(0);
    cMeanADC.SaveAs(pdfName.data());
    
}

// Print to the screen or pass on to another function a vector containing the event numbers 
// of high-count events. With roughly correct timing it is easy to find high-count events when 
// running with the source or LED but if the signal is low (for example when testing perovskites)
// it is useful to look at the sum of all ADC counts from all 64 anodes, find the mean of it 
// and look for events with sumADC higher than mean by some value
std::vector<int> PrintHighCountEventsNumbers(TTree* mdfTree, std::string quadrant, bool rotation)
{
    // Arrays to hold the ADC counts 
    short ADC[64];
    std::vector<int> highCountEvents;

    std::string branchname = GetADCBranchName(quadrant);
    TBranch* bADC = mdfTree->GetBranch(branchname.data());
    bADC->SetAddress(&ADC);

    int maxADC = 0;
    double sumADC = 0;
    std::vector<double> vSumADC;
    for (int i=0; i<bADC->GetEntries(); ++i){
        for (int anode=1; anode<=64; anode++)
        {
            bADC->GetEntry(i);
            if (ADC[anode-1] > maxADC){
                maxADC = ADC[anode-1]; 
            }
            sumADC += ADC[anode-1];
        }
        vSumADC.push_back(sumADC);
        
        if (sumADC > 2600.){
            // std::cout << "SumADC " << sumADC << std::endl;
            // std::cout << i << ", "; 
            highCountEvents.push_back(i);
            
        }
        sumADC = 0;
    }
    double totSum = std::accumulate(vSumADC.begin(), vSumADC.end(), 0.0);
    double mean = totSum/vSumADC.size();
    // std::cout << "Mean sum of ADC counts in events = " << mean << std::endl;

    return highCountEvents;
}

// 2D histogram of ADC counts for high count events given by PrintHighCountEvents() function 
// Only the selected side/quadrant will be plotted 
// Useful when looking for potential signal events when setup timing still off and 
// the trigger did not necessarily trigger at the right time
// and when interested in one side only
void PlotEvent2D(TTree* mdfTree, std::string quadrant, bool rotation, int eventno, std::string pdfName)
{
    // Arrays to hold the ADC counts 
    short ADC[64];

    std::string branchname = GetADCBranchName(quadrant);
    TBranch* bADC = mdfTree->GetBranch(branchname.data());
    bADC->SetAddress(&ADC);

    TH2F hADC("hADC", "ADC Counts", 8, 0, 8, 8, 0, 8);

    int maxADC = -1;
    int minADC = 9999;

    std::pair<int, int> binno;
    for (int anode=1; anode<=64; anode++)
    {
        bADC->GetEntry(eventno);
        binno = GetBinCoords(quadrant, anode, rotation);
        hADC.SetBinContent(binno.first, binno.second, ADC[anode-1]);
        if (ADC[anode-1] > maxADC)
            maxADC = ADC[anode-1];
        if (ADC[anode-1] < minADC)
            minADC = ADC[anode-1];
    }

    TCanvas cEvent("cEventLR", "ADC counts", 800, 800);
    hADC.Draw("colz, text");
    hADC.GetZaxis()->SetRangeUser(minADC, maxADC);
    gStyle->SetOptStat(0);
    cEvent.SaveAs(pdfName.data());
}

// 2D histograms of ADC counts for user-selected events for both sides of the setup
// WARNING: quadrant A on the left and D on the right are hard-coded here
// so if changing the used MaPMT slots make sure to update this function
void PlotSingleEvent2DLR(TTree* left, TTree* right, int eventno, std::string pdfName)
{
    // Arrays to hold the ADC counts 
    short ADCleft[64];
    short ADCright[64];

    // Retrieve branches holding the ADC counts
    TBranch* bADCleft = left->GetBranch("B01_PMT2_ADC");
    bADCleft->SetAddress(&ADCleft);
    TBranch* bADCright = right->GetBranch("B01_PMT1_ADC");
    bADCright->SetAddress(&ADCright);

    TH2F hADCleft("hADCleft", "ADC Counts left", 8, 0, 8, 8, 0, 8);
    TH2F hADCright("hADCright", "ADC Counts right", 8, 0, 8, 8, 0, 8);

    int maxADC = -1;
    int minADC = 9999;

    std::pair<int, int> binno;
    for (int anode=1; anode<=64; anode++)
    {
        bADCleft->GetEntry(eventno);
        binno = GetBinCoords("A", anode, false);
        hADCleft.SetBinContent(binno.first, binno.second, ADCleft[anode-1]);
        if (ADCleft[anode-1] > maxADC)
            maxADC = ADCleft[anode-1];
        if (ADCleft[anode-1] < minADC)
            minADC = ADCleft[anode-1];

        bADCright->GetEntry(eventno);
        binno = GetBinCoords("D", anode, true);
        hADCright.SetBinContent(binno.first, binno.second, ADCright[anode-1]);
        if (ADCright[anode-1] > maxADC)
            maxADC = ADCright[anode-1];
        if (ADCright[anode-1] < minADC)
            minADC = ADCright[anode-1];
    }

    std::cout << "Max ADC count = " << maxADC << std::endl;

    TCanvas cEventLR("cEventLR", "ADC counts", 1600, 800);
    cEventLR.Divide(2, 1);
    cEventLR.cd(1);
    hADCleft.Draw("colz, text");
    // Since plotting on the same canvas, make sure to have the same z axis range
    hADCleft.GetZaxis()->SetRangeUser(minADC, maxADC);
    cEventLR.cd(2);
    hADCright.Draw("colz, text");
    hADCright.GetZaxis()->SetRangeUser(minADC, maxADC);
    gStyle->SetOptStat(0);
    cEventLR.SaveAs(pdfName.data());

}

// Plot difference in timestamps between consecutive events 
// This function might be useful if unsure if the run is with real trigger 
// or pulser 
void PlotTimeStampDiff(TTree* mdfTree, std::string pdfName)
{
    int timestamp;
    std::vector<int> timeDiffs;
    TBranch* bTimestamp = mdfTree->GetBranch("B01_timestamp");
    bTimestamp->SetAddress(&timestamp);
    std::vector<int> times;

    for (int i=0; i<mdfTree->GetEntries(); i++){
        bTimestamp->GetEntry(i);
        times.push_back(timestamp);
        // std::cout << "Timestamp = " << timestamp << std::endl;
        if (i>0){
            timeDiffs.push_back(times.at(i) - times.at(i-1));
        }
    }

    int minTimeDiff = *std::min_element(timeDiffs.begin(), timeDiffs.end());
    int maxTimeDiff = *std::max_element(timeDiffs.begin(), timeDiffs.end());
    TH1F* hTimeDiff = new TH1F("hTimeDiff", "Time between events; Time [arbitrary unit]; Events", 100, minTimeDiff, maxTimeDiff);

    for (auto diff : timeDiffs){
        hTimeDiff->Fill(diff);
        // std::cout << "Time differences = " << diff << std::endl;
    }
    
    TCanvas cTD = TCanvas("cTD", "c", 800, 800);
    hTimeDiff->Draw("hist");
    hTimeDiff->SetTitle("");
    cTD.SaveAs(pdfName.data());
}