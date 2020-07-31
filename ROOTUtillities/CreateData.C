#include "DataTable.h"
void CreateData(){
     RootUtilities::DataTable t1;
     t1.load_from_csv("data.csv", "L8:O25", X_DATA, X_ERROR_DATA, Y_DATA, Y_ERROR_DATA);
     const int n_points1 = t1.get_length();
     auto graph1 = new TGraphErrors(n_points1,t1.get_x(),t1.get_y(),t1.get_xError(),t1.get_yError());
     graph1->SetMarkerStyle(kFullCircle);
     graph1->SetMarkerColor(kRed);

     RootUtilities::DataTable t2;
     t2.load_from_csv("data.csv", "D4:G18", X_DATA, X_ERROR_DATA, Y_DATA, Y_ERROR_DATA);
     t2.print_table();
     const int n_points2 = t2.get_length();
     auto graph2 = new TGraphErrors(n_points2,t2.get_x(),t2.get_y(),t2.get_xError(),t2.get_yError());
     graph2->SetMarkerStyle(kFullCircle);
     graph2->SetMarkerColor(kBlue);

     auto mg = new TMultiGraph();
     mg->Add(graph1);
     mg->Add(graph2);
     mg->GetXaxis()->SetTitle("T^{-1} #times 10^{3} [K^{-1}]");
     mg->GetYaxis()->SetTitle("ln(#frac{p}{p_{A}})");
     mg->Draw("AP");
     auto *f1 = new TF1("f1","x++1", t1.get_xMin(), t1.get_xMax());
     graph1->Fit("f1","R");

     auto *f2 = new TF1("f2","x++1", t2.get_xMin(), t2.get_xMax());
     f2->SetLineColor(kBlue);
     graph2->Fit("f2","R");
     auto legend = new TLegend(.65, .7, .9, .9, "Expertimental points");
     legend->AddEntry(graph1, "Heating", "PE");
     legend->AddEntry(graph2, "Cooling", "PE");
     legend->AddEntry(f1, "Heating fit", "l");
     legend->AddEntry(f2, "Cooling fit", "l");
     legend->Draw("same");
}                      
