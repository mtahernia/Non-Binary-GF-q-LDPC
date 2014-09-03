<map version="0.9.0">
<!-- To view this file, download free mind mapping software FreeMind from http://freemind.sourceforge.net -->
<node COLOR="#000000" CREATED="1409638411138" ID="ID_1726228935" MODIFIED="1409641878350" TEXT="Execution sequence">
<font NAME="SansSerif" SIZE="12"/>
<node CREATED="1409640228592" FOLDED="true" ID="ID_1905294142" MODIFIED="1409716310403" POSITION="right" TEXT="Get Input arguements from input">
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
<node CREATED="1409638441401" FOLDED="true" ID="ID_1189177112" MODIFIED="1409716312931" POSITION="right" TEXT="Create LDPC_Code Code from definition file and BlockLength">
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
<node CREATED="1409638469125" FOLDED="true" ID="ID_1762884976" MODIFIED="1409716316831" POSITION="right" TEXT="Create Channel">
<font NAME="SansSerif" SIZE="12"/>
<node CREATED="1409715562444" ID="ID_575169700" MODIFIED="1409715588986" TEXT="Channel = new AWGN_Channel"/>
<node CREATED="1409715971061" ID="ID_1545884558" MODIFIED="1409715993126" TEXT="Channel.SetNoiseSigma(noise_sigma)"/>
</node>
<node CREATED="1409638478005" FOLDED="true" ID="ID_19250453" MODIFIED="1409716318691" POSITION="right" TEXT="Code.SetChannel(*Channel)">
<font NAME="SansSerif" SIZE="12"/>
<node CREATED="1409716075079" ID="ID_859795458" MODIFIED="1409716115465" TEXT="Channel --&gt; Code.Channel"/>
</node>
<node CREATED="1409638505401" FOLDED="true" ID="ID_694243494" MODIFIED="1409716319331" POSITION="right" TEXT="Channel-&gt;ProcessMapping(Code)">
<font NAME="SansSerif" SIZE="12"/>
<node CREATED="1409716173014" ID="ID_785220020" MODIFIED="1409716193364" TEXT="Code.MapInUse.Normalize()">
<node CREATED="1409716194944" ID="ID_695900780" MODIFIED="1409716259088" TEXT="Normalize Code.Map.vals to have a power of 1"/>
</node>
</node>
<node CREATED="1409638550704" ID="ID_101428037" MODIFIED="1409641148850" POSITION="right" TEXT="Print Channel Data">
<font NAME="SansSerif" SIZE="12"/>
</node>
<node CREATED="1409638870710" ID="ID_1767232481" MODIFIED="1409641148850" POSITION="right" TEXT="Code.ResetGrapgh()">
<font NAME="SansSerif" SIZE="12"/>
<node CREATED="1409737184958" ID="ID_658129683" MODIFIED="1409737227508" TEXT="Code.Graph.Reset(BlockLength, lambda_degs, lambda_wts, rho_degs, rho_wts, MapInUse)">
<node CREATED="1409739308292" FOLDED="true" ID="ID_525258313" MODIFIED="1409739368676" TEXT="Clear()">
<node CREATED="1409739318932" ID="ID_296213944" MODIFIED="1409739337321" TEXT="delete variable_nodes"/>
<node CREATED="1409739337772" ID="ID_778458636" MODIFIED="1409739344713" TEXT="delete check_nodes"/>
<node CREATED="1409739345028" ID="ID_1143626073" MODIFIED="1409739354713" TEXT="delete edges"/>
<node CREATED="1409739356820" ID="ID_156939020" MODIFIED="1409739363809" TEXT="delete EdgeStack"/>
</node>
<node CREATED="1409740000559" ID="ID_1155715655" MODIFIED="1409740009332" TEXT="Calculate parameters">
<node CREATED="1409739372308" ID="ID_1774552626" MODIFIED="1409739484984" TEXT="Calculate Ratio=M/N=\frac{\sum\rho_i/i}{\sum\lambda_j/j}"/>
<node CREATED="1409739485899" ID="ID_998731944" MODIFIED="1409739513164" TEXT="N = Block length"/>
<node CREATED="1409739514623" ID="ID_1717638499" MODIFIED="1409739551863" TEXT="M = (int) ceil (ratio*N)"/>
<node CREATED="1409739553151" ID="ID_1857642677" MODIFIED="1409739596471" TEXT="E = (int) floor(N / \sum\lambda_j/j);"/>
</node>
<node CREATED="1409739954972" ID="ID_1713172958" MODIFIED="1409739968184" TEXT="Generate nodes and edges">
<node CREATED="1409739605622" ID="ID_523893888" MODIFIED="1409739853449" TEXT="edges = new edge[E]"/>
<node CREATED="1409739858444" ID="ID_506456160" MODIFIED="1409739882505" TEXT="variable_nodes = new variable_node[N]"/>
<node CREATED="1409739883260" ID="ID_631993653" MODIFIED="1409739905125" TEXT="check_nodes = new check_node[M]"/>
<node CREATED="1409739907256" ID="ID_704320825" MODIFIED="1409739942656" TEXT="EdgeStack = new edge*[E*2]"/>
</node>
</node>
<node CREATED="1409737247588" ID="ID_808911292" MODIFIED="1409737249688" TEXT="Variables.Init(Graph.variable_nodes, Graph.N);"/>
<node CREATED="1409737252116" ID="ID_1251313932" MODIFIED="1409737271173" TEXT="Checks.Init(Graph.check_nodes, Graph.M);"/>
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
