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
	TH1F* ratio_h = new TH1F("ratio_h","E1/(E1+E2)",100,0,1);
   TH2F* e1_vs_e2_h = new TH2F("e1_vs_e2_h","E1 vs E2",500,0,2000,500,0,2000);

// Create some variables
   Double_t	e1, e2, ratio;

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

		e1_h->Fill(e1);
		e2_h->Fill(e2);
		e1_vs_e2_h->Fill(e1,e2);
		
		ratio=e1/(e1+e2);
		ratio_h->Fill(ratio);
		
		if(i!=0 && i%1000==0) cout << i << " entries processed" << endl;
	}
	cout << entries2Process << " entries processed\nProcessing complete." << endl;
// end of main loop

// Create a canvas and draw some histograms
	cout << "Drawing histograms..." << endl;

   TCanvas* c1 = new TCanvas("c1","c1");
   c1->Divide(2,2);

   c1->cd(1);
   e1_h->Draw();
   c1->cd(2);
   e2_h->Draw();
   c1->cd(3);
   e1_vs_e2_h->Draw("colz");
   c1->cd(4);
   ratio_h->Draw();
   
   cout << endl << "Complete." << endl;

}
