﻿///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2022.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cmath>

#include <boost/version.hpp>

#if !defined(BOOST_VERSION)
#error BOOST_VERSION is not defined. Ensure that <boost/version.hpp> is properly included.
#endif

#if (BOOST_VERSION >= 108000)
#if !defined(BOOST_NO_EXCEPTIONS)
#define BOOST_NO_EXCEPTIONS
#endif
#if !defined(BOOST_NO_RTTI)
#define BOOST_NO_RTTI
#endif
#endif

#if ((BOOST_VERSION >= 107700) && !defined(BOOST_MATH_STANDALONE))
#if (defined(_MSC_VER) && (_MSC_VER < 1920))
#else
#define BOOST_MATH_STANDALONE
#endif
#endif

#if ((BOOST_VERSION >= 107900) && !defined(BOOST_MP_STANDALONE))
#define BOOST_MP_STANDALONE
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif

#if defined(__clang__) && !defined(__APPLE__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#endif

#if (BOOST_VERSION < 107900)
#include <boost/math/policies/error_handling.hpp>
#include <boost/throw_exception.hpp>
#endif

#include <boost/math/bindings/decwide_t.hpp>
#include <boost/math/special_functions/gamma.hpp>

#include <examples/example_decwide_t.h>

WIDE_DECIMAL_NAMESPACE_BEGIN

#if(__cplusplus >= 201703L)
namespace math::wide_decimal {
#else
namespace math { namespace wide_decimal { // NOLINT(modernize-concat-nested-namespaces)
#endif

namespace detail {

template<typename FloatingPointType>
auto sin_series(const FloatingPointType& x) -> FloatingPointType
{
  using floating_point_type = FloatingPointType;

        auto term        = x;
  const auto x2          = x * x;
        auto sum         = x;
        auto term_is_neg = true;
  const auto tol         = std::numeric_limits<floating_point_type>::epsilon() * x;

  for(auto k = static_cast<std::uint32_t>(UINT32_C(3));
           k < static_cast<std::uint32_t>(UINT32_C(10000));
           k = static_cast<std::uint32_t>(k + static_cast<std::uint32_t>(UINT8_C(2))))
  {
    const auto k_times_k_minus_one =
      static_cast<std::uint32_t>
      (
        k * static_cast<std::uint32_t>(k - static_cast<std::uint32_t>(UINT8_C(1)))
      );

    term *= x2;
    term /= k_times_k_minus_one;

    if(term < tol)
    {
      break;
    }

    term_is_neg ? sum -= term : sum += term;

    term_is_neg = (!term_is_neg);
  }

  return sum;
}

template<typename FloatingPointType>
auto cos_series(const FloatingPointType& x) -> FloatingPointType
{
  using floating_point_type = FloatingPointType;

  const auto x2          = x * x;
        auto term        = x2 / 2U;
        auto sum         = term;
        auto term_is_neg = true;
  const auto tol         = std::numeric_limits<floating_point_type>::epsilon() * x;

  for(auto k = static_cast<std::uint32_t>(UINT32_C(4));
           k < static_cast<std::uint32_t>(UINT32_C(10000));
           k = static_cast<std::uint32_t>(k + static_cast<std::uint32_t>(UINT8_C(2))))
  {
    term *= x2;
    term /= std::uint32_t(k * std::uint32_t(k - 1U));

    if(term < tol)
    {
      break;
    }

    term_is_neg ? sum -= term : sum += term;

    term_is_neg = (!term_is_neg);
  }

  return 1U - sum;
}

} // namespace detail

template<typename FloatingPointType>
auto sin(const FloatingPointType& x) -> FloatingPointType // NOLINT(misc-no-recursion)
{
  using floating_point_type = FloatingPointType;

  floating_point_type s;

  if(x < 0)
  {
    s = -sin(-x);
  }
  else if(x > 0)
  {
    // Perform argument reduction and subsequent scaling of the result.

    // Given x = k * (pi/2) + r, compute n = (k % 4).

    // | n |  sin(x) |  cos(x) |  sin(x)/cos(x) |
    // |----------------------------------------|
    // | 0 |  sin(r) |  cos(r) |  sin(r)/cos(r) |
    // | 1 |  cos(r) | -sin(r) | -cos(r)/sin(r) |
    // | 2 | -sin(r) | -cos(r) |  sin(r)/cos(r) |
    // | 3 | -cos(r) |  sin(r) | -cos(r)/sin(r) |

    const auto k = static_cast<std::uint32_t>     (x / boost::math::constants::half_pi<floating_point_type>());
    const auto n = static_cast<std::uint_fast32_t>(k % 4U);

    floating_point_type r = x - (boost::math::constants::half_pi<floating_point_type>() * k);

    const auto is_neg =  (n > 1U);
    const auto is_cos = ((n == 1U) || (n == 3U));

    auto n_angle_identity = static_cast<std::uint_fast32_t>(0U);

    static const floating_point_type one_tenth = floating_point_type(1U) / 10U;

    // Reduce the argument with factors of three until it is less than 1/10.
    while(r > one_tenth)
    {
      r /= 3U;

      ++n_angle_identity;
    }

    s = (is_cos ? detail::cos_series(r) : detail::sin_series(r));

    // Rescale the result with the triple angle identity for sine.
    for(auto t = static_cast<std::uint_fast32_t>(0U); t < n_angle_identity; ++t)
    {
      s = (s * 3U) - (((s * s) * s) * 4U);
    }

    s = fabs(s);

    if(is_neg)
    {
      s = -s;
    }
  }
  else
  {
    s = 0;
  }

  return s;
}

#if(__cplusplus >= 201703L)
} // namespace math::wide_decimal
#else
} // namespace wide_decimal
} // namespace math
#endif

WIDE_DECIMAL_NAMESPACE_END

namespace example009b_boost
{
  template<class T>
  auto test_sin_only() -> bool
  {
    // Table[N[Sin[i/10], 320], {i, 1, 40, 1}]
    const std::array<T, 40U> control =
    {
      T("0.099833416646828152306814198410622026989915388017982259992766861561651744283292427609662443804063036267832503180935989035450807237470459378873356101984918410496834773050632832494359789005222424086081422729667437173614296907238515782454808279553597338785346338792323336167886742490732984534800581449739604751862203867889744"),
      T("0.19866933079506121545941262711838975037020672954020540398639599139797072838116914661620815031158815790563753061147077326997451432250706773283119306542805662310956110688522144018898896133572738166827137098912914737342698676576441896105175633139380512755476788383835784448495017230870254038428967258145160322974142589976707"),
      T("0.29552020666133957510532074568502737367783211174261844850153103617326193395974630660931647890788493763333462343774851291263683294310816295401105089114655564417632484567800351736421802459746061006016588537447582067748496728805685528812489639765172090801582577470021601798712029928754290340288133904471304219163756432902014"),
      T("0.38941834230865049166631175679570526459306018344395889511584896585734711067665116920570022981124721276155692414392724248254699823010440416965494721732599999578473271063470606944800350018383339289932292910483033084331328908540850640635808343032099960019885887616388161425320890004001922369064331470980122157818875525866917"),
      T("0.47942553860420300027328793521557138808180336794060067518861661312553500028781483220963127468434826908613209108450571741781109374860994028278015396204619192460995729393228140053354633818805522859567013569985423363912107172077738015297987137716951517618072114969807370147476869703198703900097339549102989443417733111109674"),
      T("0.56464247339503535720094544565865790710988808499415177102426589426735593023496609217226419945649892870417109265981436315039156300908447683556481468821270187452583550653859165755190952723414830233328216152740429635374539202029692047841705517857437029570484831133010157259007996186962270844294772251103937051968217758221723"),
      T("0.64421768723769105367261435139872018306581384457368964474396308809382997544967566471462669216875770535830322938026758837931012921299009896152536841962607931318942607902135379854764417926576982970794170722618316661902610581990034084559568578220692451989237913208969760409593884604198504823784026801979207409119660028365589"),
      T("0.71735609089952276162717461058138536619278523779142282098968252068287843394482340713965584503376518488010301777621800699828310641363736358677725921968563848736813225785864741408195051487151962113946742289177114150308392151883332269220687556737279293432171157147206839645986706082160460683930842687548603993537777696482270"),
      T("0.78332690962748338846138231571354862314014792572030960356048515256195421632397815919979476393547632078361811722126149922690226884536066980363723086742125160409598764836669322045437939837457587697592136906219149433232630618524721741776400352324218946534034332441296112470181487734856371695285225018718105393557598147050533"),
      T("0.84147098480789650665250232163029899962256306079837106567275170999191040439123966894863974354305269585434903790792067429325911892099189888119341032772921240948079195582676660699990776401197840878273256634748480287029865615701796245539489357292467012708648628105338203056137721820386844966776167426623901338275339795676426"),
      T("0.89120736006143533995180257787170353831890931945282652766035329176720991242922777630604933441830128850539537501698304447052096523290153969680661217533520039683785536911497109034485822375238858592433292592799155090087408986002664059155189282346478299134480998384202744733619052825913121056820232138892704540246315699707502"),
      T("0.93203908596722634967013443549482599541507058820873073536659789445024234157679205421574172243811849596245202246125458170968975636474615066739426403881095793143716166397384362445476398835468988241087743826297508641592140833324770741695210191253875436296484636880337265330195583341366633371125721553603181389759436200996925"),
      T("0.96355818541719296470134863003955481534204849131773911795564922309212274180000645960003989591649229949460929402011556851740465126987831262896099420169470402678071425891796445369318819597420865495429205569942206970037962236017500028526549403902328289943091206550858912885354733525612525113075688901069355602917933788329606"),
      T("0.98544972998846018065947457880609751735626167234736563194021894560084152895833890818549995064092871368654188119062283537710255920227974277853674284648320820695790772230931611009437049333076585916456303698580804013168621745005265265376889518004358546688076896843250912057609623909971929823169606989917817582153747095273015"),
      T("0.99749498660405443094172337114148732270665142592211582194997482405934520970787064838945099773041098011758362107434377781983525546591264444329546279689323805522160638220984074127796544460850134624817768566436445817635301689308257024588280203501076619043315868613565949107333256196602810234007282890903482704365723171372349"),
      T("0.99957360304150516434211382554623417197949791475491995534260751586102935936910928218877391186740204734120746017160954256854073339829833099339646588206190062771368270008767458967884690718293459199959518652478405931290401982231763759653112580901652191379456244387964660136246035131205849533141340462222921532777948101890781"),
      T("0.99166481045246861534613339864787565240681957116712372532710249102330503573374293682962023666637768201232656475829307431319059288310463297186546309864239251498884401459611732560543826662504426438271053370475966682348105240097538524286386619317913082799755811717103422810040951183600900915394624119711316010218596970152505"),
      T("0.97384763087819518653237317884335760670293947136523395566725825917196340081095162806654878116681487243984159024318330470771551840006645931349053644395669083391578717614600760805289104174524894920881459206453999011707657574837984463894493427841886246352780779685380838820100401073800787418953138062145601648954365582260991"),
      T("0.94630008768741448848970961163495776211399866559491176443047155279581557867364093262388851338104652228144624576377488800145688780678622861081848853102757381219958901779320463713839356796027912956660190463997892302781172078302995375494472285217793223313721868906564657007289033650397110461303108285115711633444656574021197"),
      T("0.90929742682568169539601986591174484270225497144789026837897301153096730154078354462012668892495938030996789674239948626128095310867532812027002033974677378284837931019696699774984357047516517548098734245516884866266599397842058560483528737652460663019429655921188458358194895013349986918835827100625452967334980513265004"),
      T("0.86320936664887377068075931326902458492047242489508107697183045949721373321031890919647746976491022978565256396920467842090865311167123109058157631869601354644228127076055940461558200062972187252865573328393506268080199817614200389118696128001742219729649799677321838860053548526242122111696135245183496807525209335759952"),
      T("0.80849640381959018430403691041611906515855960597557707903336060873485739976370198824654132653494731812971056694925013366010276048805640686912609902510174862295802925501774191055575106619417032528144704536599002447665067659814635865759084244377163960786222773483526806913089634550971671009275245146328216516497460655753339"),
      T("0.74570521217672017738540621164349953894264877802047425750762828050000099313904725787119141718409288762817250225753133592135334980555453298342113583580957089845003916537428359514540258159501067012614643093890466791278548509412263198294482089203977890584979559621160856653150662947581690813330676541668856555926001056589318"),
      T("0.67546318055115092656577152534128337425336495789352584226890212866520453810237650287236820369272822923031431203835479683397158415560897964500442567034074871773380039466631402665146650624595962263358092605917664279533104332069760125108255118041493995068294927574357470344893978044297439024161783478008239984286066817286768"),
      T("0.59847214410395649405185470218616227170359717157722357330262703263874427219273707504021147151387635074633324297315095531879351969880823505794121464485872933758944179409484813544096314951726438100653075906829553120156168884255853896569048212036556354041783974047264245755172933459834194686575915107678539881084867930463780"),
      T("0.51550137182146423525772693520936824389387858775426312126259173008382479389654329578947094066839673017469723357482222972313569197276996466558964656433408532957898566379848245592978166223113636258005284786593808090453954254743024446303319713169929728336181691694396880183634055554162696059744184064010049912808814807617455"),
      T("0.42737988023382993455605308585788064749647642266670256499017776070511819871284819850842195294392781318212389173058752145150409445701824065239706486521728776529312972490871295704774682083283385842200821434603570870391529108287093997993075782213912726062670925960985921291447296330766760411631501907504149865620368089916360"),
      T("0.33498815015590491954385375271242210603030652888358671068410107309479432819890613054682246275158586886142742511414764842572988876664154552487512739681860517927490690160440637271213652171257967022871526802264706373243894660619861407617055190156209890178693591030424473153956031283078668842998615530226211214924794242909746"),
      T("0.23924932921398232818425691873957537221555293029961877411621026588071077867123294250811504927632446212835112700899789094709117450080997216559830837972056888652189138306136400312330159152183635946697920135139421316248202155577652712218488540374353378095318963104896736287612166192887014347194105409680605834200243441656711"),
      T("0.14112000805986722210074480280811027984693326425226558415188264123242200996701447191128217285344986375041367294826732741684445703166885757375403365785491121781178547683482078216676413721556665886468984403153833012515278359076522350444195094488983392554562224160383624182939544259174410366457405665411545993098230085116590"),
      T("0.041580662433290579194698271596673100554613422963806750648009000765884551159723457294693947210970175323414682068964755406196117853185055515536367502184858806375727393003441500191709609724536844312456189292874794619222316620120726507627631015391511349872516131832246458700541832579288158691659905465748310571094345452730674"),
      T("-0.058374143427579909137217414619095185125125099082926569709350254222736850627565515891304223718180146382958369771891687426690401817372933669628934714128999816395959135284164820297882755380153344224285849131749825437562125047819181844294673357228547396114333585032382504084632109808775453308828916474461685118941248066760955"),
      T("-0.15774569414324838201165427760248237084555143640549646739301984531414850087674140765268771756354810908497044456672771397587400003432968418600046581873638291466802179509849578327613909713574798478197733108001034018397838462295085811717925635184509351707724563700383944969790137694154364261581358170933885415388815874550951"),
      T("-0.25554110202683131924990242936373907581092037943434407750244351596873135347239660945032318621104810472004602107812633264051812110168323929248750948893462048951988215597537804832833202741494421316313570249688737104036203289877259491261665760801035102421573540709599428273567229845109251470184203497484916939389199183878459"),
      T("-0.35078322768961984812036880004363558508498173594058348541575514907064944957444117526230794153854315038554487811719106424816813190406756240268082378812984482109948666111844751200923042544039785713366329194528892768778496273792955041711667264870784798852836153441039856433724829635254152464649304298522101988323473441387780"),
      T("-0.44252044329485238426672734749269391091848782847472412742160599106592858868930270268932614672152045527307336282757785369565494338516565882008121378625178403450796910197292106185527097340422920468132070110657696118722705496505077026083999707627995559014406532500353027345261960996771224967109840926100460145338318553325098"),
      T("-0.52983614090849321321077762570120826985418868399691226932185486017092489519725855516859026329015450120967501263111872900682668045754903797617215645593604827495580655394849519823294809220951644515570082653651622054602428441556436899788030232844249423200110601162900844714159093051942008638274423514405497875340468936384822"),
      T("-0.61185789094271907573358608611888243771607580529324213205561645106564594555599881455513663711661443690317214018601268950605174665808882719416970836177861609490076911153846305831739849677585284245535016277175629397579039406223600133437963738047531477314887490408050522385164698961204072671265736799648726498243184625071063"),
      T("-0.68776615918397381809088812537868956103447279761125446525819959876123247433091905904510575797015474965666391644276620323134451165119994735926712094727095539223321889309296393768218103193276779933731359853990423397419521465590504014949348504989986482743684702170571286175589467457900187275216473929586537113016451757281626"),
      T("-0.75680249530792825137263909451182909413591288733647257148541677340131049361917941642357281056242274808769969716642307813772864757427323515257779045583094080949993321903392284375417685548870516394746329375271844371297092291461095449917737430452879922009308208676145051901855600894702056303957250150839620701647919936181974")
    };

    const T tol = std::numeric_limits<T>::epsilon() * static_cast<std::uint32_t>(UINT32_C(100000));

    bool result_is_ok = true;

    for(auto   i = static_cast<std::size_t>(UINT8_C(0U));
               i < control.size();
             ++i)
    {
      const T x = T(static_cast<std::uint8_t>(i + static_cast<std::size_t>(UINT8_C(1U)))) / 10U;
      const T s = sin(x);

      using std::fabs;
      const T closeness = fabs(1 - fabs(s / control[i])); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)

      const bool result_sin_is_ok = (closeness < tol);

      result_is_ok = (result_sin_is_ok && result_is_ok);

      if(!result_is_ok)
      {
        break;
      }
    }

    {
      // N[Sin[-123/100], 320]
      const T control_sin_minus_123_over_100("-0.94248880193169751002382356538924454146128740562765030213503850580321337526239457699475330824324143921798706558129213165475867911532250057315531384606869197267570933343720037218122749721852711690388462263929347257007713356972691000813404786065726262784830938294586267540335750707839759066203851739553593520736083704283326");

      const T s = sin(T(-123) / 100);

      using std::fabs;
      const T closeness = fabs(1 - fabs(s / control_sin_minus_123_over_100));

      const auto result_sin_minus_123_over_100_is_ok = (closeness < tol);

      result_is_ok = (result_sin_minus_123_over_100_is_ok && result_is_ok);
    }

    {
      const T s0 = sin(T(0));

      const auto result_s0_is_ok = (s0 == 0);

      result_is_ok = (result_s0_is_ok && result_is_ok);
    }

    return result_is_ok;
  }

  template<class T>
  auto test_tgamma() -> bool
  {
    // N[Gamma[5/2], 320]
    const T control_tgamma_002_and_half       ("1.3293403881791370204736256125058588870981620920917903461603558423896834634432741360312129925539084990621701177182119279996771146492933169518938202822020903013465282739898288421374438797717131196716990715344509721001309792615136097903875251426389255139390852308711844802354413316444296623040644993756797988057103001081064");

    // N[Gamma[501/2], 320]
    const T control_tgamma_250_and_half       ("2.0436157637867679327428104085816876547990204417416818422316103907295629613270921528345348459327816682636056342241950537446541535116288177757620180708816682736540856939947792019709829601955492735707432530618999787498280191875334522078610897426020922257309921502998837542955056170017022660969773746509339458003165654615581E491");

    // N[Gamma[-301/3], 320]
    const T control_tgamma_100_and_third_minus("-8.3557703333576718256158073988179208699145415756386069459318008596922002407591322112916720524641859723334916854149321144950007130289318393900505551471648382390592504702784319225368018374357793516541262525400066540804092567079990711749716708828216656334660630599555105263837919721470429933608136568647200819078822477473661E-159");

    // N[Gamma[-1/3], 320]
    const T control_tgamma_001_third_minus    ("-4.0623538182792012508358640844635413565579817981703810951820674038913488962052276250275024438589187333193044708300991765624267846577586354798215734443429685003024677468627525170477770995701556357325740345053104608510715618361049532637992095124677920741526841038682881484510994970415391794339637926102520597719997972596582");

    // N[Gamma[-2/3], 320]
    const T control_tgamma_002_third_minus    ("-4.0184078020616214504835394114620164661930340669359516514256424913856264152516157293114743335617831841287385738001083950035786268542567083899665700515627046522509347345750048275138254833707354127263331749290931765921691619586487426024184787123946997890505913760308709532709148615480237591230740512828607815485081984326040");

    // N[Gamma[-4/3], 320]
    const T control_tgamma_004_third_minus    ("3.0467653637094009381268980633476560174184863486277858213865505529185116721539207187706268328941890499894783531225743824218200884933189766098661800832572263752268508101470643877858328246776167267994305258789828456383036713770787149478494071343508440556145130779012161113383246227811543845754728444576890448289998479447436");

    // N[Gamma[-5/3], 320]
    const T control_tgamma_005_third_minus    ("2.4110446812369728702901236468772098797158204401615709908553854948313758491509694375868846001370699104772431442800650370021471761125540250339799420309376227913505608407450028965082952900224412476357999049574559059553014971751892455614510872274368198734303548256185225719625489169288142554738444307697164689291049190595624");

    const T tgamma_002_and_half        = ::boost::math::tgamma(T(T(   5U) / 2U));
    const T tgamma_250_and_half        = ::boost::math::tgamma(T(T( 501U) / 2U));
    const T tgamma_100_and_third_minus = ::boost::math::tgamma(T(T(-301)  / 3));
    const T tgamma_001_third_minus     = ::boost::math::tgamma(T(T(  -1)  / 3));
    const T tgamma_002_third_minus     = ::boost::math::tgamma(T(T(  -2)  / 3));
    const T tgamma_004_third_minus     = ::boost::math::tgamma(T(T(  -4)  / 3));
    const T tgamma_005_third_minus     = ::boost::math::tgamma(T(T(  -5)  / 3));

    using std::fabs;

    const T closeness_002_and_half        = fabs(1 - fabs(tgamma_002_and_half        / control_tgamma_002_and_half));
    const T closeness_250_and_half        = fabs(1 - fabs(tgamma_250_and_half        / control_tgamma_250_and_half));
    const T closeness_100_and_third_minus = fabs(1 - fabs(tgamma_100_and_third_minus / control_tgamma_100_and_third_minus));
    const T closeness_001_third_minus     = fabs(1 - fabs(tgamma_001_third_minus     / control_tgamma_001_third_minus));
    const T closeness_002_third_minus     = fabs(1 - fabs(tgamma_002_third_minus     / control_tgamma_002_third_minus));
    const T closeness_004_third_minus     = fabs(1 - fabs(tgamma_004_third_minus     / control_tgamma_004_third_minus));
    const T closeness_005_third_minus     = fabs(1 - fabs(tgamma_005_third_minus     / control_tgamma_005_third_minus));

    const T tol = std::numeric_limits<T>::epsilon() * static_cast<std::uint32_t>(UINT32_C(100000));

    const auto result_is_ok_002_and_half        = (closeness_002_and_half        < tol);
    const auto result_is_ok_250_and_half        = (closeness_250_and_half        < tol);
    const auto result_is_ok_100_and_third_minus = (closeness_100_and_third_minus < tol);
    const auto result_is_ok_001_third_minus =     (closeness_001_third_minus     < tol);
    const auto result_is_ok_002_third_minus =     (closeness_002_third_minus     < tol);
    const auto result_is_ok_004_third_minus =     (closeness_004_third_minus     < tol);
    const auto result_is_ok_005_third_minus =     (closeness_005_third_minus     < tol);

    const auto result_is_ok =
    (
         result_is_ok_002_and_half
      && result_is_ok_250_and_half
      && result_is_ok_100_and_third_minus
      && result_is_ok_001_third_minus
      && result_is_ok_002_third_minus
      && result_is_ok_004_third_minus
      && result_is_ok_005_third_minus
    );

    return result_is_ok;
  }

  template<class T>
  auto test_both() -> bool
  {
    return test_sin_only<T>() && test_tgamma<T>();
  }
} // namespace example009b_boost

#if defined(WIDE_DECIMAL_NAMESPACE)
auto WIDE_DECIMAL_NAMESPACE::math::wide_decimal::example009b_boost_math_standalone() -> bool
#else
auto math::wide_decimal::example009b_boost_math_standalone() -> bool
#endif
{
  using wide_decimal_010_type = math::wide_decimal::decwide_t<static_cast<std::int32_t>(INT32_C( 10)), std::uint32_t, void>;
  using wide_decimal_035_type = math::wide_decimal::decwide_t<static_cast<std::int32_t>(INT32_C( 35)), std::uint32_t, void>;
  using wide_decimal_105_type = math::wide_decimal::decwide_t<static_cast<std::int32_t>(INT32_C(105)), std::uint32_t, void>;
  using wide_decimal_305_type = math::wide_decimal::decwide_t<static_cast<std::int32_t>(INT32_C(305)), std::uint32_t, void>;

  #if (BOOST_VERSION < 107900)
  using boost_wrapexcept_round_type  = ::boost::wrapexcept<::boost::math::rounding_error>;
  using boost_wrapexcept_domain_type = ::boost::wrapexcept<std::domain_error>;
  #endif

  auto result_is_ok = false;

  #if (BOOST_VERSION >= 108000)
  #else
  try
  {
  #endif
  const auto result_010_is_ok = example009b_boost::test_both<wide_decimal_010_type>();
  const auto result_035_is_ok = example009b_boost::test_both<wide_decimal_035_type>();
  const auto result_105_is_ok = example009b_boost::test_both<wide_decimal_105_type>();
  const auto result_305_is_ok = example009b_boost::test_both<wide_decimal_305_type>();

  result_is_ok = (   result_010_is_ok
                  && result_035_is_ok
                  && result_105_is_ok
                  && result_305_is_ok);
  #if (BOOST_VERSION >= 108000)
  #else
  }
  #if (BOOST_VERSION < 107900)
  catch(const boost_wrapexcept_round_type& e)
  {
    result_is_ok = false;

    std::cout << "Exception: boost_wrapexcept_round_type: " << e.what() << std::endl;
  }
  catch(const boost_wrapexcept_domain_type& e)
  {
    result_is_ok = false;

    std::cout << "Exception: boost_wrapexcept_domain_type: " << e.what() << std::endl;
  }
  #else
  // LCOV_EXCL_START
  catch(const ::boost::math::rounding_error& e)
  {
    result_is_ok = false;

    std::cout << "Exception: ::boost::math::rounding_error: " << e.what() << std::endl;
  }
  catch(const std::domain_error& e)
  {
    result_is_ok = false;

    std::cout << "Exception: std::domain_error: " << e.what() << std::endl;
  }
  // LCOV_EXCL_STOP
  #endif
  #endif

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if defined(WIDE_DECIMAL_STANDALONE_EXAMPLE009B_BOOST_MATH_STANDALONE)

#include <iomanip>
#include <iostream>

// TBD: Handle exception catching in example009b_boost_math_standalone at a later time.
auto main() -> int // NOLINT(bugprone-exception-escape)
{
  const auto result_is_ok = math::wide_decimal::example009b_boost_math_standalone();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif

#if defined(__clang__) && !defined(__APPLE__)
#pragma GCC diagnostic pop
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#endif
