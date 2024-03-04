void deltaE()
{
  using namespace RooFit;
  gROOT->LoadMacro("~/Belle2Style.C");

  TCanvas *canvas = new TCanvas("canvas", "canvas", 600,800);
  canvas->Draw();
  canvas->cd();

  TFile *input = new TFile("/home/belle2/khalid/Root_files/pion_gen/piongen_Mbc_fit.root");
  TTree *mytree = (TTree*)input->Get("mytree");
  //TLatex Tl;

  RooRealVar deltaE("deltaE","deltaE",-0.1,0.1);
  RooDataSet data("data","dataset", mytree, RooArgList(deltaE));

  //signal
  RooRealVar mean("mean","mean",-0.000463447, -0.1, 0.1);
  RooRealVar sigma("sigma", "sigma", 0.00775666, 0.0001, 1);
  RooGaussian sig("sig","signal p.d.f.", deltaE, mean, sigma);
  RooRealVar no_sig("no_sig", "no_sig", 45000.0, 10000.0, 100000.0);
  
  //bkg
  RooRealVar c0("c0", "coefficient #0",  -0.0112536, -1.0, 1.0);
  RooRealVar c1("c1", "coefficient #1", -0.241572, -1.0, 1.0);
  RooRealVar c2("c2", "coefficient #2", -0.662185, -1.0, 1.0);
  RooRealVar c3("c3", "coefficient #3", 0.1, -1.0, 1.0);
  RooRealVar c4("c4", "coefficient #4", -0.1, -1.0, 1.0);
  RooChebychev bkg("bkg", "background p.d.f.", deltaE, RooArgList(c0, c1, c2, c3, c4));
  RooRealVar no_bkg("no_bkg","no_bkg", 5000.0, 0., 100000.0);

  //model
  RooAddPdf model("model","model", RooArgList(sig,bkg), RooArgList(no_sig,no_bkg));
  model.fitTo(data);
    
    
  canvas->cd();
  TPad *pad1 = new TPad("pad1","data pad",0.1,0.5,1.,1.);
  pad1->Draw();
  pad1->cd();
  RooPlot *frame = deltaE.frame();
  data.plotOn(frame);
  model.plotOn(frame, Components(sig), LineColor(kRed), LineStyle(kDashed));
  model.plotOn(frame, Components(bkg), LineColor(kGreen), LineStyle(kDashed));
  model.plotOn(frame);
  frame->SetTitle("#bar{B^{0}} #rightarrow D^{+} (K^{-} #pi^{+} #pi^{+}) #pi^{-} ");
  frame->GetYaxis()->SetTitle("Events");
  frame->GetXaxis()->SetTitle("#Delta E [GeV]");
  gPad->SetLeftMargin(0.2);
  frame->Draw();
  canvas->Update();


  canvas->cd();
  TPad *pad2 = new TPad("pad2","pull pad",0.1,0.3,1.,0.5);
  pad2->Draw();
  pad2->cd();
  RooHist* hpull = frame->pullHist();
  RooPlot* frame2 = deltaE.frame();
  frame2->addPlotable(hpull,"P");
  frame2->SetTitle("");
  frame2->GetYaxis()->SetTitle("Pull");
  frame2->GetXaxis()->SetTitleSize(0.1);
  frame2->GetXaxis()->SetLabelSize(0.1);
  frame2->GetYaxis()->SetTitleSize(0.1);
  frame2->GetYaxis()->SetLabelSize(0.1);
  frame2->GetYaxis()->SetTitleOffset(0.5);
  frame2->GetYaxis()->SetNdivisions(402);
  frame2->SetAxisRange(-4., 4.,"Y");
  frame2->GetXaxis()->SetTitle("#Delta E [GeV]");
  gPad->SetLeftMargin(0.2);
  gPad->SetBottomMargin(0.1);
  frame2->Draw();

  TLine *line = new TLine(-0.1,0,0.1,0);
  line->SetLineColor(kBlack);
  line->Draw();
  
  canvas->cd();
  TPad *pad3 = new TPad("pad3","residual pad",0.1,0.1,1.,0.3);
  pad3->Draw();
  pad3->cd();
  RooHist* hresid = frame->residHist() ;
  RooPlot* frame3 = deltaE.frame();
  frame3->addPlotable(hresid,"P");
  frame3->SetTitle("");
  frame3->GetYaxis()->SetTitle("Residual");
  frame3->GetXaxis()->SetTitleSize(0.1);
  frame3->GetXaxis()->SetLabelSize(0.1);
  frame3->GetYaxis()->SetTitleSize(0.1);
  frame3->GetYaxis()->SetLabelSize(0.1);
  frame3->GetYaxis()->SetTitleOffset(0.5);
  frame3->GetYaxis()->SetNdivisions(10);
  frame3->SetAxisRange(-250., 250.,"Y");
  frame3->GetXaxis()->SetTitle("#Delta E [GeV]");
  gPad->SetLeftMargin(0.2);
  gPad->SetBottomMargin(0.1);
  frame3->Draw();

  TLine *line1 = new TLine(-0.1,0,0.1,0);
  line->SetLineColor(kBlack);
  line->Draw();
  
  
  cout << "chi^2/ndf = " << frame->chiSquare() << endl;
  data.Print();
  cout << " sigma =" << sigma.getValV() << endl << endl;
  cout << " mean =" << mean.getValV() << endl << endl;
  cout << " no_sig =" << no_sig.getValV() << endl << endl;




  pad1->cd();
  TLatex* mean_txt = new TLatex();
  mean_txt->DrawLatex(-0.09, 5000, "#scale[0.8]{#color[6]{mean = 0.0002006 GeV}}");
  mean_txt->Draw();
  TLatex* sigma_txt = new TLatex();
  sigma_txt->DrawLatex(-0.09, 4000, "#scale[0.8]{#color[6]{#sigma = 0.008518 GeV}}");
  sigma_txt->Draw();
  TLatex* yield_txt = new TLatex();
  yield_txt->DrawLatex(-0.09, 3000, "#scale[0.8]{#color[6]{signal yield = 52876}}");
  yield_txt->Draw();
  TLatex* chi_txt = new TLatex();
  chi_txt->DrawLatex(-0.09, 2000, "#scale[0.8]{#color[6]{#chi^{2} = 1.677}}");
  chi_txt->Draw();

  canvas->cd();
    
  canvas->SaveAs("deltaE_fit.pdf");
}
