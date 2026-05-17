import com.thoughtworks.selenium.DefaultSelenium;
import com.thoughtworks.selenium.Selenium;
import org.junit.After;
import org.junit.Assert;
import org.junit.Test;

public class StatcounterChromeRcTest {
    private Selenium selenium;

    private void start() {
        selenium = new DefaultSelenium("localhost", 4444, "*chrome", "https://statcounter.com/");
        selenium.start();
        selenium.windowMaximize();
    }

    @After
    public void stop() {
        if (selenium != null) {
            selenium.stop();
        }
    }

    @Test
    public void publicPagesAreAvailableInChrome() {
        start();
        check("/", "//h1[contains(normalize-space(.),'Understand your visitors')]");
        check("/features/", "//h1[contains(normalize-space(.),'Features')]");
        check("/pricing/", "//h1[contains(normalize-space(.),'Flexible pricing')]");
        check("/how-it-works/", "//h1[contains(normalize-space(.),'How does Statcounter work')]");
        check("/about/recommendations/", "//*[contains(normalize-space(.),'Customer testimonials')]");
        check("/success-stories/", "//h1[contains(normalize-space(.),'Success Stories')]");
        check("/free-hit-counter/", "//h1[contains(normalize-space(.),'Free Hit Counter')]");
        check("/free-invisible-web-tracker/", "//h1[contains(normalize-space(.),'Free Invisible Web Tracker')]");
        check("/free-web-stats/", "//h1[contains(normalize-space(.),'Free Web Stats')]");
        check("/statcounter-vs-google-analytics/", "//h1[contains(normalize-space(.),'Statcounter vs Google Analytics')]");
        check("/install-guides/", "//*[contains(normalize-space(.),'Installation Guides for Popular Platforms')]");
        check("/support/faq/", "//*[contains(normalize-space(.),'FAQ')]");
        check("/about/mission/", "//*[contains(normalize-space(.),'Reliable Service')]");
        check("/ip-address-tracker/", "//*[contains(normalize-space(.),'IP ADDRESS TRACKER')]");
        check("/support/using-statcounter/", "//*[contains(normalize-space(.),'Analyze Your First Visitors')]");
    }

    private void check(String path, String xpath) {
        selenium.open(path);
        waitForXpath(xpath);
    }

    private void waitForXpath(String xpath) {
        for (int i = 0; i < 30; i++) {
            if (selenium.isElementPresent("xpath=" + xpath)) {
                return;
            }
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                throw new AssertionError(e);
            }
        }
        Assert.fail("Element was not found by XPath: " + xpath);
    }
}
