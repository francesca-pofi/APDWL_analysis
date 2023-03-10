#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TStyle.h"



int main( int argc, char* argv[] ) {

  if( argc==1 ) {
    std::cout << "USAGE: ./signal-noise_comparison [fileName]" << std::endl;
    exit(1);
  }

  std::string fileName(argv[1]);

  std::cout << "-> Opening file: " << fileName << std::endl;

  TString fileName_tstr(fileName);


  TFile *output_f=new TFile("signal-noise_comparison.root","RECREATE");

  TFile* file = TFile::Open( fileName.c_str() );
  TTree* tree = (TTree*)file->Get("tree");

  TFile* file_ref = TFile::Open("../../noise/analysis/Run_APDWL_HV405_noise_K_Measurements_Only_1_12_2023.root");
  TTree* tree_ref = (TTree*)file_ref->Get("tree");


  float vamp; 
  float vamp_ref;
  tree->SetBranchAddress( "vamp", &vamp );
  tree_ref->SetBranchAddress( "vamp", &vamp_ref );


  TH1D* h1 = new TH1D( "vamp", "", 500, -0.1, 0.1 );

  int nentries = tree->GetEntries();

  for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

    tree->GetEntry(iEntry);
    h1->Fill( vamp );

  }
  std::cout<<"first histo filled"<<std::endl;
  gStyle->SetOptStat(0);

  TCanvas* c1 = new TCanvas( "c1", "", 1200., 800. );
  c1->cd();

  TLegend* legend = new TLegend(0.7, 0.75, 0.9, 0.9, "HV = 405 V");

 // h1->Scale(1./nentries);     //metodo2
  h1->Scale(1./h1->Integral("width"));  //metodo1
 // h1->DrawNormalized("HISTO");    //metodo4

  TH1D* h2 = new TH1D( "vamp_ref", "", 300, -0.04, 0.1);

  int nentries_ref = tree_ref->GetEntries();

  for( unsigned iEntry=0; iEntry<nentries_ref; ++iEntry ) {

    tree_ref->GetEntry(iEntry);
    h2->Fill( vamp_ref );

  }
  std::cout<<"second histo filled"<<std::endl;
  gStyle->SetOptStat(0);
  c1->cd();


  h2->Scale(1./h2->Integral("width"));   //metodo1
 // h2->Scale(1./nentries_ref);    //metodo2
 // h2->Scale(h1->Integral()/h2->Integral()); //metodo3

  h2->SetLineColor(2);
  h1->SetLineColor(4); 

  gStyle->SetOptStat(0);

  h2->Draw("HISTO"); 
  h1->Draw("HISTO SAME");
 //h2->DrawNormalized( "HISTO SAME");   //metodo4

  legend->AddEntry(h2, "noise");
  legend->AddEntry(h1, "signal"); 
  legend->Draw("SAME");

  output_f->cd();
  c1->Write();
  output_f->Close();

  std::cout << "-> Tree saved in: " << output_f->GetName() << std::endl;

  return 0;
} 

