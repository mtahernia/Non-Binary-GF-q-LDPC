<map version="0.9.0">
<!-- To view this file, download free mind mapping software FreeMind from http://freemind.sourceforge.net -->
<node COLOR="#000000" CREATED="1409638411138" ID="ID_1726228935" MODIFIED="1409641878350" TEXT="Execution sequence">
<font NAME="SansSerif" SIZE="12"/>
<node CREATED="1409640228592" ID="ID_1905294142" MODIFIED="1409641876002" POSITION="right" TEXT="Get Input arguements from input">
<font NAME="SansSerif" SIZE="12"/>
<node CREATED="1409640255004" ID="ID_368535862" MODIFIED="1409641147177" TEXT="iterations">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409640264100" ID="ID_96172482" MODIFIED="1409641147177" TEXT="inputfilename">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409640270988" ID="ID_652399738" MODIFIED="1409641147177" TEXT="BlockLength">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409640277980" ID="ID_352138514" MODIFIED="1409641147177" TEXT="count_runs">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409640285680" ID="ID_781260334" MODIFIED="1409641147177" TEXT="ChannelType">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409640318988" ID="ID_1954985084" MODIFIED="1409641147177" TEXT="OutputFile">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409640346800" ID="ID_1143337776" MODIFIED="1409641147176" TEXT="Seed">
<font NAME="SansSerif" SIZE="12"/>
</node>
</node>
<node CREATED="1409638424480" ID="ID_1071844953" MODIFIED="1409641148849" POSITION="right" TEXT="Open output file">
<richcontent TYPE="NOTE"><html>
  <head>
    
  </head>
  <body>
    <p>
      ReportBuf.OpenFile(OutputLogFileName);
    </p>
  </body>
</html></richcontent>
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409640427863" ID="ID_1638641049" MODIFIED="1409641148849" POSITION="right" TEXT="Initialize Seed">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409640461695" ID="ID_1564564232" MODIFIED="1409641148849" POSITION="right" TEXT="Open Definition FIle">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409638441401" ID="ID_1189177112" MODIFIED="1409641148850" POSITION="right" TEXT="Create LDPC_Code Code from definition file and BlockLength">
<font NAME="SansSerif" SIZE="12"/>
<node CREATED="1409713454801" ID="ID_1946364355" MODIFIED="1409713522824" TEXT="Set Block Length"/>
<node CREATED="1409713523640" ID="ID_979399261" MODIFIED="1409713573329" TEXT="LDPC_Code::GetFromFile">
<node CREATED="1409713576139" ID="ID_1310065297" MODIFIED="1409714699224" TEXT="rho_degs, rho_wts"/>
<node CREATED="1409714685935" ID="ID_1634249420" MODIFIED="1409714687932" TEXT="lambda_degs, lambda_wts "/>
<node CREATED="1409714758682" ID="ID_1940587605" MODIFIED="1409714791349" TEXT="MapInUse.GetFromFile(file)">
<node CREATED="1409714851814" ID="ID_58836200" MODIFIED="1409714871767" TEXT="MapInUse.q &lt;-- file"/>
<node CREATED="1409714793358" ID="ID_1151214198" MODIFIED="1409714882719" TEXT="MapInUse.vals &lt;-- file"/>
</node>
<node CREATED="1409714700707" ID="ID_804087397" MODIFIED="1409714928104" TEXT="MapInUse.GetQ() --&gt; GFq::q"/>
</node>
</node>
<node CREATED="1409638469125" ID="ID_1762884976" MODIFIED="1409641148850" POSITION="right" TEXT="Create Channel">
<font NAME="SansSerif" SIZE="12"/>
<node CREATED="1409715562444" ID="ID_575169700" MODIFIED="1409715588986" TEXT="Channel = new AWGN_Channel"/>
</node>
<node CREATED="1409638478005" ID="ID_19250453" MODIFIED="1409641148850" POSITION="right" TEXT="Code.SetChannel(*Channel)">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409638505401" ID="ID_694243494" MODIFIED="1409641148850" POSITION="right" TEXT="Channel-&gt;ProcessMapping(Code)">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409638550704" ID="ID_101428037" MODIFIED="1409641148850" POSITION="right" TEXT="Print Channel Data">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409638870710" ID="ID_1767232481" MODIFIED="1409641148850" POSITION="right" TEXT="Code.ResetGrapgh()">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409638889634" ID="ID_696602883" MODIFIED="1409641148850" POSITION="right" TEXT="Code.GetZeroCodeword(Codeword);">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409638900814" ID="ID_214529080" MODIFIED="1409641148850" POSITION="right" STYLE="fork" TEXT="Channel-&gt;SimulateOutputVector(Codeword, ChannelOutput);">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409638925762" ID="ID_1699898478" MODIFIED="1409641148850" POSITION="right" TEXT="Code.Init_Messages(ChannelOutput);">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409638927498" ID="ID_1163776652" MODIFIED="1409641148850" POSITION="right" STYLE="fork" TEXT="AccumulatedSER += Code.Belief_Propagation_Decoder(iterations);">
<font NAME="SansSerif" SIZE="12"/>
</node>
</node>
</map>
