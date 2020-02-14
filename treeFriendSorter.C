#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>

void treeFriendSorter() {

// Change the files you want to process here
	TFile *f1 = new TFile("Cs1.0.root");
	TFile *f2 = new TFile("Cs1.1.root");

// Create some histograms
	TH1F* e1_h = new TH1F("e1_h","E1",1000,0,2000);
	TH1F* e2_h = new TH1F("e2_h","E2",1000,0,2000);
	TH1F* eSum_h = new TH1F("eSum_h","E1+E2",1000,0,4000);
	TH1F* eCorr_h = new TH1F("eCorr_h","Corrected Energy",1000,0,4000);
	TH1F* ratio_h = new TH1F("ratio_h","E1/(E1+E2)",100,0,1);
   TH2F* e1_vs_e2_h = new TH2F("e1_vs_e2_h","E1 vs E2",500,0,2000,500,0,2000);
   
   TH2F* eSum_vs_ratio_h = new TH2F("eSum_vs_ratio_h","(E1 + E2) vs E1/(E1+E2)",100,0,1,500,0,4000);
   TH2F* eCorr_vs_ratio_h = new TH2F("eCorr_vs_ratio_h","Corrected energy vs E1/(E1+E2)",100,0,1,500,0,4000);
      
   TH2F* corrFac_vs_ratio_h = new TH2F("corrFac_vs_ratio_h","(E1+E2)/1800 vs E1/(E1+E2)",100,0,1,500,0,2);
   TH2F* peak_vs_ratio_h = new TH2F("peak_vs_ratio_h","(E1 + E2) vs E1/(E1+E2), photopeak only",100,0,1,500,0,2);
	TGraph* peak_vs_ratio_g = new TGraph();

// Create some variables
   Double_t	e1, e2, eSum, eCorr, ratio;
   
// Fit coefficients for energy correction
   Double_t a0=1.23819;
   Double_t a1=-0.526534;

// Set up the tree branches
	TTree* T1 = (TTree*)f1->Get("tree");
	TTree* T2 = (TTree*)f2->Get("tree");
   T1->SetBranchAddress("gaussmax", &e1);
   T2->SetBranchAddress("gaussmax", &e2);
   T1->AddFriend(T2);
   
	Long64_t nentries1 = T1->GetEntriesFast();
	Long64_t nentries2 = T2->GetEntriesFast();
	if(nentries1!=nentries2) cout << "Error! Files do not contain the same number of entries.\n This will cause problems if you try to process them." << endl;

	Long64_t entries2Process = 2000;  // Use this to only process part of the file.
//	Long64_t entries2Process = nentries1; // Use this to process all of the file

   cout << nentries1 << " entries in file: " << f1->GetName()  << endl;
   cout << nentries2 << " entries in file: " << f2->GetName()  << endl;
   cout << endl << "Processing " << entries2Process << " entries." << endl;

// Loop over entries in file
	for (Long64_t i=0;i<entries2Process;i++) {
		T1->GetEntry(i);

		eSum=e1+e2;
	
		e1_h->Fill(e1);
		e2_h->Fill(e2);
		eSum_h->Fill(eSum);
		e1_vs_e2_h->Fill(e1,e2);
		
		ratio=e1/(e1+e2);
		ratio_h->Fill(ratio);
		
		eSum_vs_ratio_h->Fill(ratio,eSum);
		corrFac_vs_ratio_h->Fill(ratio,eSum/1800);
		if(eSum>1700 && eSum<2000) {
			peak_vs_ratio_h->Fill(ratio,eSum/1800);
			peak_vs_ratio_g->Set(peak_vs_ratio_g->GetN()+1);
			peak_vs_ratio_g->SetPoint(peak_vs_ratio_g->GetN()-1,ratio,eSum/1800);
		}
		
		eCorr=eSum/(a0+a1*ratio);
		eCorr_h->Fill(eCorr);
		eCorr_vs_ratio_h->Fill(ratio,eCorr);
		
		if(i!=0 && i%1000==0) cout << i << " entries processed" << endl;
	}
	cout << entries2Process << " entries processed\nProcessing complete." << endl;
// end of main loop

// Create a canvas and draw some histograms
	cout << "Drawing histograms..." << endl;

   TCanvas* c1 = new TCanvas("c1","c1");
   c1->Divide(3,2);

   c1->cd(1);
   e1_h->Draw();
   c1->cd(2);
   e2_h->Draw();
   c1->cd(3);
   eSum_h->Draw();
   c1->cd(4);
   e1_vs_e2_h->Draw("colz");
   c1->cd(5);
   ratio_h->Draw();
   c1->cd(6);
   eSum_vs_ratio_h->Draw("colz");
   
   TCanvas* c2 = new TCanvas("c2","c2");
   c2->Divide(2,2);
   c2->cd(1);
   corrFac_vs_ratio_h->Draw("colz");
   c2->cd(2);
   peak_vs_ratio_g->Draw("AP");
   c2->cd(3);
   eCorr_vs_ratio_h->Draw("colz");
   c2->cd(4);
   eCorr_h->SetLineColor(2);
   eCorr_h->Draw();
   eSum_h->Draw("same");
   
   cout << endl << "Complete." << endl;

}
